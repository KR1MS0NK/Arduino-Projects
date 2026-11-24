#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID"; // Your Wi-Fi SSID
const char* password = "YOUR_PASSWORD"; // Your Wi-Fi Password

WebServer server(80); // Object to manage HTTP requests, listening on port 80

const int ledPin = 2; // GPIO where the LED is connected

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Initialize LED pin as output
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");
  server.on("/on", HTTP_GET, []() {
    digitalWrite(ledPin, HIGH); // Turn LED on
    server.send(200, "text/plain", "LED is on");
  });
  server.on("/off", HTTP_GET, []() {
    digitalWrite(ledPin, LOW); // Turn LED off
    server.send(200, "text/plain", "LED is off");
  });
  server.begin(); // Start the server
}

void loop() {
  server.handleClient(); // Handle client requests
}
