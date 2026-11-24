#include <esp_now.h>
#include <WiFi.h>

// Replace with the Slave's MAC address
uint8_t slaveAddress[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  bool ledState;
} struct_message;

// Create a struct_message called myData
struct_message myData;

const int buttonPin = 0; // GPIO where the pushbutton is connected

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, slaveAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    myData.ledState = true;
    esp_now_send(slaveAddress, (uint8_t *) &myData, sizeof(myData));
    delay(200); // Debounce delay
  } else {
    myData.ledState = false;
    esp_now_send(slaveAddress, (uint8_t *) &myData, sizeof(myData));
  }
}
