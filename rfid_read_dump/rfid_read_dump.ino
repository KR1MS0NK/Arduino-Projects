#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key defaultKey;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(50);

  // Prepare factory default key (FF FF FF FF FF FF) for MIFARE Classic tags
  for (byte i = 0; i < 6; i++) {
    defaultKey.keyByte[i] = 0xFF;
  }

  Serial.println(F("========================================"));
  Serial.println(F("MFRC522 Reader Ready - Tap any card/tag"));
  Serial.println(F("========================================"));
}

void loop() {
  // Wait for a card to enter the RF field
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  Serial.println(F("\n--- Card Detected ---"));

  // 1. Print UID (supports 4-byte, 7-byte, and 10-byte UIDs)
  Serial.print(F("UID ("));
  Serial.print(mfrc522.uid.size);
  Serial.print(F(" bytes): "));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) Serial.print(F("0"));
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) Serial.print(F(":"));
  }
  Serial.println();

  // 2. Print Card / PICC Type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print(F("Card Type:  "));
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // 3. Read Stored Data based on Card Type
  if (piccType == MFRC522::PICC_TYPE_MIFARE_UL) {
    readNtagOrUltralight();
  } else if (piccType == MFRC522::PICC_TYPE_MIFARE_MINI ||
             piccType == MFRC522::PICC_TYPE_MIFARE_1K ||
             piccType == MFRC522::PICC_TYPE_MIFARE_4K) {
    readMifareClassic(piccType);
  } else {
    Serial.println(F("Memory dump not supported for this PICC type."));
  }

  Serial.println(F("---------------------"));

  // Halt communication so the card isn't re-read continuously while held on the reader
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(1500);
}

void readNtagOrUltralight() {
  Serial.println(F("\n[NTAG / Ultralight User Memory]"));

  // NTAG213 has 144 user bytes (Pages 4 to 39).
  // MIFARE_Read reads 4 pages (16 bytes) at a time + 2 CRC bytes.
  byte userMem[144];
  uint16_t bytesRead = 0;

  for (byte page = 4; page <= 36; page += 4) {
    byte buffer[18];
    byte size = sizeof(buffer);

    MFRC522::StatusCode status = mfrc522.MIFARE_Read(page, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      // Stop gracefully if tag has fewer pages (e.g., standard 64-byte Ultralight)
      break;
    }

    memcpy(&userMem[bytesRead], buffer, 16);
    bytesRead += 16;
  }

  if (bytesRead == 0) {
    Serial.println(F("Failed to read user pages."));
    return;
  }

  // Decode NDEF URI if present
  decodeNdefUri(userMem, bytesRead);

  // Print Hex + ASCII dump of user pages
  Serial.println(F("Raw User Data (Pages 4+):"));
  for (uint16_t offset = 0; offset < bytesRead; offset += 16) {
    byte startPage = 4 + (offset / 4);
    Serial.print(F("P"));
    if (startPage < 10) Serial.print(F("0"));
    Serial.print(startPage);
    Serial.print(F("-P"));
    if (startPage + 3 < 10) Serial.print(F("0"));
    Serial.print(startPage + 3);
    Serial.print(F(": "));
    printHexAndAscii(&userMem[offset], 16);
  }
}

void decodeNdefUri(const byte* data, uint16_t len) {
  uint16_t idx = 0;

  while (idx < len) {
    byte tlvTag = data[idx++];

    if (tlvTag == 0x00) {
      continue; // NULL TLV padding
    } else if (tlvTag == 0xFE) {
      break;    // Terminator TLV
    } else if (tlvTag == 0x03) {
      // NDEF Message TLV found
      if (idx >= len) return;
      byte ndefLen = data[idx++];
      if (ndefLen == 0 || idx + ndefLen > len) return;

      byte header = data[idx++];
      byte tnf = header & 0x07;
      byte typeLen = data[idx++];
      byte payloadLen = data[idx++];

      // Check for Well-Known (TNF 0x01) URI Record ('U' = 0x55)
      if (tnf == 0x01 && typeLen == 1 && data[idx] == 0x55 && payloadLen >= 1) {
        idx++; // Skip 'U'
        byte prefixCode = data[idx++];

        Serial.print(F("Decoded NDEF URL: "));
        switch (prefixCode) {
          case 0x01: Serial.print(F("http://www.")); break;
          case 0x02: Serial.print(F("https://www.")); break;
          case 0x03: Serial.print(F("http://")); break;
          case 0x04: Serial.print(F("https://")); break;
          case 0x05: Serial.print(F("tel:")); break;
          case 0x06: Serial.print(F("mailto:")); break;
          default: break;
        }

        for (byte i = 0; i < payloadLen - 1 && idx < len; i++) {
          Serial.write(data[idx++]);
        }
        Serial.println(F("\n"));
      }
      return;
    } else {
      // Skip other TLVs (e.g., Lock Control 0x01, Memory Control 0x02)
      if (idx >= len) return;
      byte skipLen = data[idx++];
      idx += skipLen;
    }
  }
}

void readMifareClassic(MFRC522::PICC_Type piccType) {
  byte numSectors = (piccType == MFRC522::PICC_TYPE_MIFARE_MINI) ? 5 : 16;
  Serial.println(F("\n[MIFARE Classic Data Blocks]"));

  for (byte sector = 0; sector < numSectors; sector++) {
    byte firstBlock = sector * 4;
    byte trailerBlock = firstBlock + 3;

    // Authenticate sector using default Key A
    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &defaultKey, &(mfrc522.uid)
    );

    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Sector "));
      Serial.print(sector);
      Serial.print(F(": Auth failed ("));
      Serial.print(mfrc522.GetStatusCodeName(status));
      Serial.println(F(")"));
      continue;
    }

    // Read the 3 data blocks in this sector (skip sector trailer block 3)
    for (byte b = 0; b < 3; b++) {
      byte blockAddr = firstBlock + b;
      if (blockAddr == 0) continue; // Skip Block 0 (manufacturer UID block)

      byte buffer[18];
      byte size = sizeof(buffer);
      status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);

      if (status == MFRC522::STATUS_OK) {
        // Only print blocks that contain non-zero data
        bool isEmpty = true;
        for (byte i = 0; i < 16; i++) {
          if (buffer[i] != 0x00) {
            isEmpty = false;
            break;
          }
        }

        if (!isEmpty) {
          Serial.print(F("Blk "));
          if (blockAddr < 10) Serial.print(F("0"));
          Serial.print(blockAddr);
          Serial.print(F(": "));
          printHexAndAscii(buffer, 16);
        }
      }
    }
  }
}

void printHexAndAscii(const byte* data, byte length) {
  for (byte i = 0; i < length; i++) {
    if (data[i] < 0x10) Serial.print(F("0"));
    Serial.print(data[i], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F(" | "));
  for (byte i = 0; i < length; i++) {
    if (data[i] >= 32 && data[i] <= 126) {
      Serial.write(data[i]);
    } else {
      Serial.print(F("."));
    }
  }
  Serial.println();
}
