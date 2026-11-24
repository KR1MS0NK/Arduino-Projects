#include <LoRa.h> 


String inString = "";    // string to hold input
int val = 0;
int SyncWord = 0xF3;
 
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else {
    Serial.print("connected");
    }
   LoRa.setSpreadingFactor(12);           // ranges from 6-12,default 7 see API docs
   LoRa.setSignalBandwidth(62.5E3);           // for -139dB (page - 112)
   LoRa.setCodingRate4(8);                   // for -139dB (page - 112)
   LoRa.setSyncWord(SyncWord);           // ranges from 0-0xFF, default 0x12, see API docs
/*
  Serial.print("current spreading factor : ");
  Serial.println(LoRa.getSpreadingFactor());
  Serial.print("current bandwidth : ");
  Serial.println(LoRa.getSignalBandwidth());
  Serial.println("LoRa init succeeded.");
  */
}
bool i=0;
int priviousValue = 0;
int liveValue = 0;


void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
    Serial.print("Received: ");
    Serial.println(received);
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
  }
  delay(100);
}
