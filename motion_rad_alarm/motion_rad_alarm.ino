
//LCD I2C initial
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//ultrasound initialize
const int trigPin = 3;
const int echoPin = 4;
float duration, distance;

//PIR initialize
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

//buzzers initialize
int buzzer1 = 11;
int buzzer2 = 12;

int button = 10;



void detect(){
  lcd.setCursor(3,0);
  lcd.print("Motion detected!");
  digitalWrite(buzzer1,HIGH);
  digitalWrite(buzzer2,HIGH);
  delay(10000);
  digitalWrite(buzzer1,LOW);
  digitalWrite(buzzer2,LOW);
  }

void setup() {

  pinMode(sensor, INPUT);    // initialize sensor as an input

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer2, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  
  Serial.begin(9600);        // initialize serial
}


void loop()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("System Active");
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  Serial.print("Distance: ");
  Serial.println(distance);

 val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
//    digitalWrite(led, HIGH);   // turn LED ON
    delay(100);                // delay 100 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH

      if(distance <=50){
        detect();
        }
    }
  } 
  else {

      delay(200);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }

  

  
}
