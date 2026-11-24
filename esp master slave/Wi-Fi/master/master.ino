#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_SSID"; // Your Wi-Fi SSID
const char* password = "YOUR_PASSWORD"; // Your Wi-Fi Password

const int buttonPin = 0; // GPIO where the pushbutton is connected
const char* slaveIp = "http://192.168.1.XXX/"; // IP address of the slave ESP32

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // Initialize button pin as input with pull-up
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if(buttonState == LOW) { // Check if button is pressed
    sendCommand("on"); // Send "on" command to slave
    delay(1000); // Simple debounce
  } else {
    sendCommand("off"); // Send "off" command to slave
  }
  delay(10);
}

void sendCommand(String command) {
  HTTPClient http;
  http.begin(slaveIp + command); // Specify the URL
  int httpCode = http.GET(); // Send the request
  if (httpCode > 0) { // Check the returning code
    String payload = http.getString(); // Get the request response payload
    Serial.println(payload); // Print the response payload
  }
  http.end(); // Close connection
}
