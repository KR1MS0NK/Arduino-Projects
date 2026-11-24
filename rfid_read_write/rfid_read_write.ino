#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN 9

// Buffer large enough for NTAG216 (888 bytes) and MIFARE Classic 1K (752 bytes)
const uint16_t MAX_STORAGE_BYTES = 900;
uint8_t cardBuffer[MAX_STORAGE_BYTES];

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key defaultKey;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(50);

  for (byte i = 0; i < 6; i++) {
    defaultKey.keyByte[i] = 0xFF;
  }

  Serial.println(F("================================================"));
  Serial.println(F("MFRC522 Read & Serial Overwrite Ready"));
  Serial.println(F("Set Serial Monitor line ending to 'Newline'"));
  Serial.println(F("Tap a card to inspect its current data..."));
  Serial.println(F("================================================"));
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Save scanned UID to verify the same card is written to later
  byte savedUid[10];
  byte savedUidSize = mfrc522.uid.size;
  memcpy(savedUid, mfrc522.uid.uidByte, savedUidSize);

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  Serial.println(F("\n================ CARD DETECTED ================"));
  Serial.print(F("UID:       "));
  printUid(savedUid, savedUidSize);
  Serial.print(F("Card Type: "));
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  uint16_t capacity = 0;
  bool isNtag = false;

  if (piccType == MFRC522::PICC_TYPE_MIFARE_UL) {
    isNtag = true;
    capacity = getNtagCapacity();
    Serial.print(F("Capacity:  "));
    Serial.print(capacity);
    Serial.println(F(" bytes (User Memory)"));
    readAndDisplayNtag(capacity);
  } else if (piccType == MFRC522::PICC_TYPE_MIFARE_MINI ||
             piccType == MFRC522::PICC_TYPE_MIFARE_1K ||
             piccType == MFRC522::PICC_TYPE_MIFARE_4K) {
    isNtag = false;
    capacity = (piccType == MFRC522::PICC_TYPE_MIFARE_MINI) ? 224 : 752;
    Serial.print(F("Capacity:  "));
    Serial.print(capacity);
    Serial.println(F(" bytes (Writable Data Blocks)"));
    readAndDisplayClassic(capacity);
  } else {
    Serial.println(F("Unsupported card type for reading/writing."));
    mfrc522.PICC_HaltA();
    delay(1500);
    return;
  }

  // Flush any leftover characters in the Serial buffer before prompting
  while (Serial.available() > 0) {
    Serial.read();
  }

  Serial.println(F("------------------------------------------------"));
  Serial.println(F("Type new data in Serial Monitor & press Enter to overwrite"));
  Serial.println(F("(Or type CANCEL to skip):"));

  String userInput = waitForSerialLine();

  if (userInput.equalsIgnoreCase("CANCEL")) {
    Serial.println(F("Operation cancelled. Card was not modified."));
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
    return;
  }

  // Check storage limits and prepare payload buffer
  uint16_t requiredBytes = 0;
  memset(cardBuffer, 0x00, sizeof(cardBuffer));

  if (isNtag) {
    requiredBytes = buildNdefPayload(userInput, cardBuffer, MAX_STORAGE_BYTES);
  } else {
    requiredBytes = userInput.length();
    if (requiredBytes <= MAX_STORAGE_BYTES) {
      memcpy(cardBuffer, userInput.c_str(), requiredBytes);
    }
  }

  if (requiredBytes > capacity) {
    Serial.println(F("\n[ERROR] Input exceeds available card storage!"));
    Serial.print(F("  Required:  "));
    Serial.print(requiredBytes);
    Serial.println(isNtag ? F(" bytes (includes NDEF formatting)") : F(" bytes"));
    Serial.print(F("  Available: "));
    Serial.print(capacity);
    Serial.println(F(" bytes"));
    Serial.println(F("Write aborted. Nothing was overwritten."));
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1500);
    return;
  }

  // Ensure the card is still present (or prompt user if they lifted it while typing)
  if (!ensureCardReady(savedUid, savedUidSize)) {
    Serial.println(F("[ERROR] Wrong card detected or timeout. Write aborted."));
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1500);
    return;
  }

  Serial.print(F("Writing "));
  Serial.print(requiredBytes);
  Serial.print(F(" / "));
  Serial.print(capacity);
  Serial.println(F(" bytes..."));

  bool success = isNtag ? writeNtagMemory(capacity) : writeClassicMemory(capacity);

  if (success) {
    Serial.println(F("[SUCCESS] Card data overwritten!"));
  } else {
    Serial.println(F("[ERROR] Write failed. Keep the card steady on the antenna and try again."));
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  Serial.println(F("\nRemove card or tap another card to continue..."));
  delay(2000);
}

// ---------------------------------------------------------------------------
// Capacity & Card Presence Helpers
// ---------------------------------------------------------------------------

uint16_t getNtagCapacity() {
  // Send NTAG GET_VERSION command (0x60) to distinguish NTAG213 / 215 / 216
  byte cmd = 0x60;
  byte verBuf[12];
  byte verLen = sizeof(verBuf);

  if (mfrc522.PCD_TransceiveData(&cmd, 1, verBuf, &verLen) == MFRC522::STATUS_OK && verLen >= 8) {
    switch (verBuf[6]) {
      case 0x0F: return 144; // NTAG213 (36 user pages)
      case 0x11: return 504; // NTAG215 (126 user pages)
      case 0x13: return 888; // NTAG216 (222 user pages)
    }
  }

  // Fallback: check Capability Container (Page 3, Byte 2) or default to Ultralight (48B)
  byte pageBuf[18];
  byte size = sizeof(pageBuf);
  if (mfrc522.MIFARE_Read(0, pageBuf, &size) == MFRC522::STATUS_OK) {
    // Bytes 12..15 correspond to Page 3
    if (pageBuf[12] == 0xE1 && pageBuf[14] > 0) {
      uint16_t ccSize = (uint16_t)pageBuf[14] * 8;
      return (ccSize <= MAX_STORAGE_BYTES) ? ccSize : MAX_STORAGE_BYTES;
    }
  }
  return 48; // Standard MIFARE Ultralight
}

bool ensureCardReady(const byte* expectedUid, byte expectedSize) {
  // Quick test read on Page/Block 0 to see if the card is still sitting active on the reader
  byte testBuf[18];
  byte size = sizeof(testBuf);
  if (mfrc522.MIFARE_Read(0, testBuf, &size) == MFRC522::STATUS_OK) {
    return true;
  }

  // Card was lifted while typing; ask user to place it back
  Serial.println(F("Place the same card back on the reader to complete writing..."));
  unsigned long startWait = millis();
  while (millis() - startWait < 15000) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      if (mfrc522.uid.size == expectedSize &&
          memcmp(mfrc522.uid.uidByte, expectedUid, expectedSize) == 0) {
        return true;
      }
      return false; // Different card presented
    }
    delay(50);
  }
  return false;
}

String waitForSerialLine() {
  while (true) {
    if (Serial.available() > 0) {
      String line = Serial.readStringUntil('\n');
      line.replace("\r", "");
      if (line.length() > 0) {
        return line;
      }
    }
    delay(20);
  }
}

// ---------------------------------------------------------------------------
// Read Functions
// ---------------------------------------------------------------------------

void readAndDisplayNtag(uint16_t capacity) {
  memset(cardBuffer, 0x00, sizeof(cardBuffer));
  uint16_t totalPages = capacity / 4;
  uint16_t bytesRead = 0;

  for (uint16_t offsetPages = 0; offsetPages < totalPages; offsetPages += 4) {
    byte pageAddr = 4 + offsetPages;
    byte temp[18];
    byte size = sizeof(temp);
    if (mfrc522.MIFARE_Read(pageAddr, temp, &size) != MFRC522::STATUS_OK) {
      break;
    }
    uint16_t chunk = min((uint16_t)16, (uint16_t)(capacity - bytesRead));
    memcpy(&cardBuffer[bytesRead], temp, chunk);
    bytesRead += chunk;
  }

  Serial.print(F("Stored Data: "));
  if (!decodeAndPrintNdef(cardBuffer, bytesRead)) {
    printPrintableSummary(cardBuffer, bytesRead);
  }
}

void readAndDisplayClassic(uint16_t capacity) {
  memset(cardBuffer, 0x00, sizeof(cardBuffer));
  uint16_t bytesRead = 0;

  for (byte sector = 0; sector < 16 && bytesRead < capacity; sector++) {
    byte firstBlock = sector * 4;
    byte trailerBlock = firstBlock + 3;

    if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &defaultKey, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
      bytesRead += (sector == 0) ? 32 : 48;
      continue;
    }

    for (byte b = 0; b < 3 && bytesRead < capacity; b++) {
      byte blockAddr = firstBlock + b;
      if (blockAddr == 0) continue; // Skip manufacturer UID block

      byte temp[18];
      byte size = sizeof(temp);
      if (mfrc522.MIFARE_Read(blockAddr, temp, &size) == MFRC522::STATUS_OK) {
        memcpy(&cardBuffer[bytesRead], temp, 16);
      }
      bytesRead += 16;
    }
  }

  Serial.print(F("Stored Data: "));
  printPrintableSummary(cardBuffer, bytesRead);
}

bool decodeAndPrintNdef(const uint8_t* data, uint16_t len) {
  uint16_t idx = 0;
  while (idx < len) {
    byte tag = data[idx++];
    if (tag == 0x00) continue;
    if (tag == 0xFE) break;
    if (tag == 0x03 && idx < len) {
      uint16_t ndefLen = data[idx++];
      if (ndefLen == 0xFF) {
        if (idx + 2 > len) return false;
        ndefLen = ((uint16_t)data[idx] << 8) | data[idx + 1];
        idx += 2;
      }
      if (ndefLen == 0 || idx + ndefLen > len) return false;

      byte header = data[idx++];
      bool shortRecord = (header & 0x10) != 0;
      byte tnf = header & 0x07;
      byte typeLen = data[idx++];

      uint32_t payloadLen = 0;
      if (shortRecord) {
        payloadLen = data[idx++];
      } else {
        if (idx + 4 > len) return false;
        payloadLen = ((uint32_t)data[idx] << 24) | ((uint32_t)data[idx + 1] << 16) |
                     ((uint32_t)data[idx + 2] << 8) | data[idx + 3];
        idx += 4;
      }

      if (tnf == 0x01 && typeLen == 1 && idx < len) {
        byte recordType = data[idx++];
        if (recordType == 'U' && payloadLen >= 1) {
          byte prefix = data[idx++];
          Serial.print(F("[NDEF URL] "));
          switch (prefix) {
            case 0x01: Serial.print(F("http://www.")); break;
            case 0x02: Serial.print(F("https://www.")); break;
            case 0x03: Serial.print(F("http://")); break;
            case 0x04: Serial.print(F("https://")); break;
            case 0x05: Serial.print(F("tel:")); break;
            case 0x06: Serial.print(F("mailto:")); break;
            default: break;
          }
          for (uint32_t i = 0; i < payloadLen - 1 && idx < len; i++) {
            Serial.write(data[idx++]);
          }
          Serial.println();
          return true;
        } else if (recordType == 'T' && payloadLen >= 1) {
          byte status = data[idx++];
          byte langLen = status & 0x3F;
          idx += langLen; // Skip language code (e.g., "en")
          Serial.print(F("[NDEF Text] "));
          for (uint32_t i = 0; i < payloadLen - 1 - langLen && idx < len; i++) {
            Serial.write(data[idx++]);
          }
          Serial.println();
          return true;
        }
      }
      return false;
    } else if (idx < len) {
      byte skip = data[idx++];
      idx += skip;
    }
  }
  return false;
}

void printPrintableSummary(const uint8_t* data, uint16_t len) {
  bool hasData = false;
  for (uint16_t i = 0; i < len; i++) {
    if (data[i] != 0x00) {
      hasData = true;
      break;
    }
  }
  if (!hasData) {
    Serial.println(F("<Empty / Blank>"));
    return;
  }

  for (uint16_t i = 0; i < len; i++) {
    if (data[i] == 0x00) continue;
    if (data[i] >= 32 && data[i] <= 126) {
      Serial.write(data[i]);
    } else {
      Serial.print('.');
    }
  }
  Serial.println();
}

// ---------------------------------------------------------------------------
// NDEF Formatting & Write Functions
// ---------------------------------------------------------------------------

uint16_t buildNdefPayload(const String& text, uint8_t* outBuf, uint16_t maxBuf) {
  const char* raw = text.c_str();
  bool isUri = false;
  uint8_t prefixCode = 0x00;
  const char* body = raw;

  // Automatically encode links/phone/email as NDEF URI; everything else as NDEF Text
  if (strncmp(raw, "https://www.", 12) == 0)      { isUri = true; prefixCode = 0x02; body = raw + 12; }
  else if (strncmp(raw, "http://www.", 11) == 0)  { isUri = true; prefixCode = 0x01; body = raw + 11; }
  else if (strncmp(raw, "https://", 8) == 0)      { isUri = true; prefixCode = 0x04; body = raw + 8; }
  else if (strncmp(raw, "http://", 7) == 0)       { isUri = true; prefixCode = 0x03; body = raw + 7; }
  else if (strncmp(raw, "tel:", 4) == 0)          { isUri = true; prefixCode = 0x05; body = raw + 4; }
  else if (strncmp(raw, "mailto:", 7) == 0)       { isUri = true; prefixCode = 0x06; body = raw + 7; }

  uint16_t bodyLen = strlen(body);
  // URI payload = [prefixCode] + body; Text payload = [0x02, 'e', 'n'] + body
  uint16_t payloadLen = isUri ? (1 + bodyLen) : (3 + bodyLen);
  bool shortRecord = (payloadLen <= 255);
  uint16_t recordLen = (shortRecord ? 4 : 7) + payloadLen;
  uint16_t tlvHeaderLen = (recordLen < 255) ? 2 : 4;
  uint16_t totalBytes = tlvHeaderLen + recordLen + 1; // +1 for 0xFE Terminator TLV

  // Return actual required size even if it overflows so the caller can display the exact error
  if (totalBytes > maxBuf) {
    return totalBytes;
  }

  uint16_t idx = 0;
  outBuf[idx++] = 0x03; // NDEF Message TLV Tag
  if (recordLen < 255) {
    outBuf[idx++] = (uint8_t)recordLen;
  } else {
    outBuf[idx++] = 0xFF;
    outBuf[idx++] = (uint8_t)(recordLen >> 8);
    outBuf[idx++] = (uint8_t)(recordLen & 0xFF);
  }

  outBuf[idx++] = shortRecord ? 0xD1 : 0xC1; // MB=1, ME=1, SR=1/0, TNF=0x01
  outBuf[idx++] = 0x01;                      // Type Length = 1

  if (shortRecord) {
    outBuf[idx++] = (uint8_t)payloadLen;
  } else {
    outBuf[idx++] = 0x00;
    outBuf[idx++] = 0x00;
    outBuf[idx++] = (uint8_t)(payloadLen >> 8);
    outBuf[idx++] = (uint8_t)(payloadLen & 0xFF);
  }

  if (isUri) {
    outBuf[idx++] = 'U';
    outBuf[idx++] = prefixCode;
  } else {
    outBuf[idx++] = 'T';
    outBuf[idx++] = 0x02; // UTF-8, 2-byte language code
    outBuf[idx++] = 'e';
    outBuf[idx++] = 'n';
  }

  memcpy(&outBuf[idx], body, bodyLen);
  idx += bodyLen;
  outBuf[idx++] = 0xFE; // Terminator TLV

  return idx;
}

bool writeNtagMemory(uint16_t capacity) {
  // Write all user pages (4 bytes each), zero-padding unused space to wipe old data
  uint16_t totalPages = capacity / 4;
  for (uint16_t p = 0; p < totalPages; p++) {
    byte pageAddr = 4 + p;
    MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(pageAddr, &cardBuffer[p * 4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Write failed at page "));
      Serial.println(pageAddr);
      return false;
    }
  }
  return true;
}

bool writeClassicMemory(uint16_t capacity) {
  uint16_t offset = 0;
  for (byte sector = 0; sector < 16 && offset < capacity; sector++) {
    byte firstBlock = sector * 4;
    byte trailerBlock = firstBlock + 3;

    if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &defaultKey, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
      Serial.print(F("Auth failed on sector "));
      Serial.println(sector);
      return false;
    }

    for (byte b = 0; b < 3 && offset < capacity; b++) {
      byte blockAddr = firstBlock + b;
      if (blockAddr == 0) continue; // Never overwrite Block 0

      if (mfrc522.MIFARE_Write(blockAddr, &cardBuffer[offset], 16) != MFRC522::STATUS_OK) {
        Serial.print(F("Write failed at block "));
        Serial.println(blockAddr);
        return false;
      }
      offset += 16;
    }
  }
  return true;
}

void printUid(const byte* uid, byte len) {
  for (byte i = 0; i < len; i++) {
    if (uid[i] < 0x10) Serial.print('0');
    Serial.print(uid[i], HEX);
    if (i < len - 1) Serial.print(':');
  }
  Serial.println();
}
