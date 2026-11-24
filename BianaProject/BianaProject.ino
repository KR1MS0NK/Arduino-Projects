#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// RFID Configuration
MFRC522DriverPinSimple ss_pin(5);  // ESP32 GPIO 21 for SS pin
MFRC522DriverSPI driver{ss_pin};    // Create SPI driver
MFRC522 mfrc522{driver};            // Create MFRC522 instance

// Hardware Configuration
/*
 SDA PIN 5
 SCK PIN 18
 MOSI PIN 23
 MISO PIN 19
 IRQ PIN DONT CONNECT
 RST PIN 21
*/
#define RED_PIN     12     // RGB LED pins
#define GREEN_PIN   13
#define BLUE_PIN    14

#define BUZZER_PIN  15     // Buzzer pin
#define RELAY_PIN   22     // Relay control pin

// Override card UID - replace with your actual override card's UID
byte overrideCard[] = {0xDE, 0xAD, 0xBE, 0xEF};

byte storedCard[4] = {0, 0, 0, 0}; // Buffer for storing the card UID
int scanCount = 0;                 // Tracks number of scans

void setup() {
  Serial.begin(115200);
  while (!Serial);       // Wait for serial port to connect (needed for some boards)
  
  // Initialize RFID reader
  mfrc522.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println(F("System ready. Scan your RFID card..."));
  
  // Initialize GPIO pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  // Initial state: LED green, relay off
  setColor(0, 255, 0);    // Green
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Print card UID to serial
  Serial.print(F("Card UID:"));
  MFRC522Debug::PrintUID(Serial, mfrc522.uid);
  Serial.println();
  
  // Convert UID to String for easier comparison (optional)
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidString += "0";
    }
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(uidString);
  
  // Check if this is the override card
  if (isOverrideCard(mfrc522.uid.uidByte)) {
    handleOverride();
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
    return;
  }
  
  // Main logic
  if (scanCount == 0) {
    // First scan - store card, turn on relay, set LED to red
    storeCard(mfrc522.uid.uidByte);
    digitalWrite(RELAY_PIN, HIGH);
    setColor(255, 0, 0); // Red
    scanCount = 1;
    Serial.println(F("Card stored. Relay ON. System armed."));
  } 
  else if (scanCount == 1) {
    if (isSameCard(mfrc522.uid.uidByte, storedCard)) {
      // Valid card - turn off relay, reset system
      digitalWrite(RELAY_PIN, LOW);
      setColor(0, 255, 0); // Green
      clearStoredCard();
      scanCount = 0;
      Serial.println(F("Valid card. Relay OFF. System disarmed."));
    } else {
      // Wrong card - sound alarm
      Serial.println(F("Wrong card! Alarm triggered."));
      triggerAlarm();
    }
  }
  
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
}

bool isOverrideCard(byte* card) {
  for (byte i = 0; i < 4; i++) {
    if (card[i] != overrideCard[i]) {
      return false;
    }
  }
  return true;
}

void handleOverride() {
  digitalWrite(RELAY_PIN, LOW);
  setColor(0, 255, 0); // Green
  clearStoredCard();
  scanCount = 0;
  Serial.println(F("Override card used. System reset."));
  beep(3, 100); // Short beeps to acknowledge override
}

void storeCard(byte* card) {
  for (byte i = 0; i < 4; i++) {
    storedCard[i] = card[i];
  }
}

void clearStoredCard() {
  for (byte i = 0; i < 4; i++) {
    storedCard[i] = 0;
  }
}

bool isSameCard(byte* card1, byte* card2) {
  for (byte i = 0; i < 4; i++) {
    if (card1[i] != card2[i]) {
      return false;
    }
  }
  return true;
}

void triggerAlarm() {
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { // 5 seconds
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
  }
}
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void beep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    if (i < times - 1) delay(duration);
  }
}
