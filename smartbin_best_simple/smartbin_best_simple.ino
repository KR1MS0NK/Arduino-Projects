#include <Servo.h>

const int trigPin  = 8;
const int echoPin  = 9; 
float duration, distance;  // bin level check

const int trigPin1  = 10;
const int echoPin1  = 11; 
float duration1, distance1;  // prox check

const int servoPin = 6;  // open and close

int LEDR = 2; // full bin
int LEDY = 3; // half bin
int LEDG = 4; // empty bin 

int buzz = 5; // full sound

// defines variables
int pos = 0;
Servo myServo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  myServo.attach(6);  // attaches the servo on pin 7 to the servo object

  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, LOW);
  pinMode(LEDY, OUTPUT);
  digitalWrite(LEDY, LOW);
  pinMode(LEDG, OUTPUT);
  digitalWrite(LEDG, LOW);
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
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
  Serial.println(distance);          // this whole code checks the waste level



    digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1*.0343)/2;
  
  Serial.print("Proximity: ");
  Serial.println(distance1);   // checks proximity to the bin


  if(distance1 <= 20)
  {
  //for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(90);              // tell servo to go to position in variable 'pos'
    //}
  }

  else{
  //for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo.write(0);              // tell servo to go to position in variable 'pos'
    //}
  }


  if(distance >=20){
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDY, LOW);
    digitalWrite(LEDR, LOW);
  }

  if(distance <=17){
    digitalWrite(LEDY, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, LOW);
  }

  if(distance <=10){
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDY, LOW);
    digitalWrite(LEDG, LOW);

//    digitalWrite(buzz, HIGH);
//    delay(1000);
//    digitalWrite(buzz, LOW);
//    delay(1000);
//    digitalWrite(buzz, HIGH);
//    delay(1000);
//    digitalWrite(buzz, LOW);
//    delay(1000);
  }
  delay(500);
}
  
