#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool ledState;
} struct_message;

// Create a struct_message called myData
struct_message myData;

const int ledPin = 2; // GPIO where the LED is connected

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  digitalWrite(ledPin, myData.ledState ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register for receive callback
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing needed here
}
