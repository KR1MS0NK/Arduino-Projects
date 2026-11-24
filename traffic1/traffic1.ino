void setup() {
  // put your setup code here, to run once:
pinMode(8, OUTPUT); //red led
pinMode(10, OUTPUT); //yellow led
pinMode(12, OUTPUT); //green led
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(8,HIGH);
delay(3000);
digitalWrite(10,HIGH);
delay(1000);

digitalWrite(8,LOW);
digitalWrite(10,LOW);

digitalWrite(12,HIGH);
delay(3000);
digitalWrite(12,LOW);
delay(500);

digitalWrite(12,HIGH);
delay(500);
digitalWrite(12,LOW);
delay(500);

digitalWrite(12, HIGH);
delay(500);
digitalWrite(12,LOW);
delay(500);

digitalWrite(12, HIGH);
delay(500);
digitalWrite(12, LOW);
delay(1000);
}
