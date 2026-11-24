/*code for using the piezoelectric plate as a sensor using the analog ports
the piezoelectric plate is connected to the analog ort A0 with the actuator port(led)at D12
the small potential difference produced by the piezoelectric late is detected by the arduino analog port
when potential is detected above a certain value, system is triggered*/

int sensorPin = A0;    // select the input pin for the ppiezoelectricplate positive terminal
int ledPin = 12;      // select the pin for the actuator
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  if(sensorValue >=100){
    digitalWrite(ledPin, HIGH);
    Serial.println("Detected!");
    delay(2000);
    digitalWrite(ledPin, LOW);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  delay(300);
}
