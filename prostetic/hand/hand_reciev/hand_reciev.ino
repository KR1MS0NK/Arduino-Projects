#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>



#define led0 16               //D0

DynamicJsonBuffer jsonBuffer;

const char *ssid      = "ssid";
const char *password  = "password";

int sensorValue0 = 0;        
       
String sensor_values;

ESP8266WebServer server(80);

void handleSentVar() {

  if (server.hasArg("sensor_reading"))
  {
    sensor_values = server.arg("sensor_reading");
    Serial.println(sensor_values);
  }
  JsonObject& root = jsonBuffer.parseObject(sensor_values);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  if (root.success())
  {
    sensorValue0          = root["sensor0_reading"].as<int>();

  }

  Serial.println(sensorValue0);

  toggle_leds();

  server.send(200, "text/html", "Data received");
}


void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  pinMode(led0, OUTPUT);
  digitalWrite(led0, HIGH);
  delay(500);
  digitalWrite(led0, LOW);
  delay(500);
  digitalWrite(led0, HIGH);
  delay(500);
  digitalWrite(led0, LOW);
  delay(500);

  toggle_leds();                 //turn off all leds as all the sensor values are zero
  
  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
}

void loop() {
  server.handleClient();
}

void toggle_leds()
{
  if (sensorValue0 == 0)  digitalWrite(led0, LOW);

  if (sensorValue0 == 1)  digitalWrite(led0, HIGH);

}
