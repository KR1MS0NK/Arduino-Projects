
const int trigPin = 4;
const int echoPin = 5;
float duration, distance;

int buzzer = 3;

void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer,OUTPUT);
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


if (distance <= 20){
  digitalWrite(buzzer, HIGH);
  }

else{
  digitalWrite(buzzer, LOW);
  }
  delay(50);
}
