
const int trigPin = 4;
const int echoPin = 5;
float duration, distance;
int ledPin=10;

void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

    Serial.print("Distance: ");
  Serial.println(distance);

  if(distance <=20){
    digitalWrite(ledPin,HIGH);
  }else{
    digitalWrite(ledPin,LOW);
    }
  
delay(500);
  
}
