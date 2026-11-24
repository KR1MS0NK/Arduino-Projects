#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN 9

// Change this to your business card URL (max ~137 chars for NTAG213)
const char* TARGET_URL = "https://example.com";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(50);

  Serial.println(F("========================================"));
  Serial.println(F("Place a blank NTAG213 card on the reader"));
  Serial.print(F("URL to write: "));
  Serial.println(TARGET_URL);
  Serial.println(F("========================================"));
}

void loop() {
  // Wait for a new card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Confirm the tag is NTAG / MIFARE Ultralight compatible (SAK == 0x00)
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_UL) {
    Serial.print(F("Error: Unsupported tag type ("));
    Serial.print(mfrc522.PICC_GetTypeName(piccType));
    Serial.println(F("). Use an NTAG213/215/216 tag."));
    mfrc522.PICC_HaltA();
    delay(1500);
    return;
  }

  Serial.println(F("\nNTAG detected! Writing NDEF URL..."));

  if (writeNdefUrl(TARGET_URL)) {
    Serial.println(F("SUCCESS: URL written! Remove card and tap it with a phone."));
  } else {
    Serial.println(F("FAILED: Could not write to tag. Keep the card steady and retry."));
  }

  // Halt PICC so it doesn't repeatedly write while sitting on the reader
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(2000);
}

bool writeNdefUrl(const char* url) {
  // NTAG213 has 36 user pages (Pages 4 to 39) = 144 bytes total
  const uint8_t MAX_USER_BYTES = 144;
  uint8_t buffer[MAX_USER_BYTES];
  memset(buffer, 0x00, sizeof(buffer));

  // 1. Compress standard URI prefixes per NFC Forum URI Record spec
  uint8_t prefixCode = 0x00; // No prefix
  const char* urlBody = url;

  if (strncmp(url, "https://www.", 12) == 0) {
    prefixCode = 0x02;
    urlBody = url + 12;
  } else if (strncmp(url, "http://www.", 11) == 0) {
    prefixCode = 0x01;
    urlBody = url + 11;
  } else if (strncmp(url, "https://", 8) == 0) {
    prefixCode = 0x04;
    urlBody = url + 8;
  } else if (strncmp(url, "http://", 7) == 0) {
    prefixCode = 0x03;
    urlBody = url + 7;
  }

  uint8_t bodyLen = strlen(urlBody);
  uint8_t payloadLen = 1 + bodyLen;      // 1 byte prefix code + URL body
  uint8_t ndefRecordLen = 4 + payloadLen; // Header + TypeLen + PayloadLen + 'U' + Payload
  uint16_t totalBytes = 2 + ndefRecordLen + 1; // TLV Tag + TLV Len + Record + Terminator

  if (totalBytes > MAX_USER_BYTES) {
    Serial.println(F("Error: URL is too long for NTAG213 (144-byte limit)."));
    return false;
  }

  // 2. Build the NDEF TLV message
  uint8_t idx = 0;
  buffer[idx++] = 0x03;          // NDEF Message TLV Tag
  buffer[idx++] = ndefRecordLen; // NDEF Message Length
  buffer[idx++] = 0xD1;          // Record Header: MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
  buffer[idx++] = 0x01;          // Type Length: 1 byte
  buffer[idx++] = payloadLen;    // Payload Length
  buffer[idx++] = 0x55;          // Record Type: 'U' (URI)
  buffer[idx++] = prefixCode;    // URI Prefix Code

  memcpy(&buffer[idx], urlBody, bodyLen);
  idx += bodyLen;

  buffer[idx++] = 0xFE;          // Terminator TLV Tag

  // 3. Ensure Page 3 (Capability Container) is configured for NTAG213 NDEF
  uint8_t ccPage[4] = {0xE1, 0x10, 0x12, 0x00};
  MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(3, ccPage, 4);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Page 3 (CC) write error: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  // 4. Write NDEF data 4 bytes per page starting at Page 4
  uint8_t pagesToWrite = (idx + 3) / 4;
  for (uint8_t p = 0; p < pagesToWrite; p++) {
    uint8_t pageAddr = 4 + p;
    status = mfrc522.MIFARE_Ultralight_Write(pageAddr, &buffer[p * 4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Write failed at page "));
      Serial.print(pageAddr);
      Serial.print(F(": "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return false;
    }
  }

  return true;
}
