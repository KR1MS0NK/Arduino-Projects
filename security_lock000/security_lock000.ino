//code by KRIMSONkay
#include <arduino-timer.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define BUZZER 2 //buzzer pin
const int trigPin = 6;
const int echoPin = 7;
float duration, distance;

int prox = 20;
int checkt = 7000;

auto timer = timer_create_default();
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


void setup() {
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  //ultrasound
  
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, LOW);   //green led
  
  pinMode(LED_R, OUTPUT);
  digitalWrite(LED_R, LOW);   //red led
  
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);   //buzzer

  timer.at(checkt, intrude);

  Serial.println("Put your card to the reader...");
  Serial.println();
    Serial.begin(600);   // Initiate a serial communication

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  
  Serial.print("Distance: ");
  Serial.println(distance);

   // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if(distance <= prox)   // main code
  { 
    timer.tick(); // tick the timer
    intrude();
    timer.cancel();

    if (content.substring(1) == "C5 64 43 B3") //change here the UID of the card/cards that you want to give access
  {
    allow();
    }
 
    }  
}

void intrude(){
  digitalWrite(LED_R, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(3000);
  digitalWrite(BUZZER, LOW);
}

void allow(){
  Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    digitalWrite(BUZZER, HIGH);
     delay(500);
     digitalWrite(BUZZER, LOW);
     delay(500);
     digitalWrite(BUZZER, HIGH);
     delay(500);
     digitalWrite(BUZZER, LOW);
     delay(500);
     digitalWrite(LED_G, LOW);

    
}
