#include <WiFi.h>


int btn0 = 23;
int testLED = 22;

int buttonState = 0;  

const char *ssid = "ssid";
const char *password = "password";


int sensorValue0 = 0;       //sensor value, I'm usingg 0/1 button state

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(btn0, INPUT);
  digitalWrite(btn0, LOW);

  pinMode(testLED, OUTPUT);    //test if the led is active
  digitalWrite(testLED, HIGH);
  delay(500);
  digitalWrite(testLED, LOW);
  delay(500);
  digitalWrite(testLED, HIGH);
  delay(500);
  digitalWrite(testLED, LOW);
  delay(500);


  // set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
}

void loop() {
  buttonState = digitalRead(btn0);


   if (buttonState == HIGH) {
    sensorValue0 = 1;
    digitalWrite(testLED, HIGH);
    Serial.println("led on");
  } 
  
  else if (buttonState == LOW){
    sensorValue0 = 0;
    digitalWrite(testLED, LOW);
    Serial.println("led off");
  }
  




  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.4.1";            //default IP address
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request. Something like /data/?sensor_reading=123
  String url = "/data?";
  url += "sensor0_reading=" + String(sensorValue0);


  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}
