int left=5;
int right=9;

void setup() {
  // put your setup code here, to run once:
  pinMode(left, OUTPUT);
  digitalWrite(left, LOW);
  pinMode(right, OUTPUT);
  digitalWrite(right, LOW);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  digitalWrite(left, LOW);
  digitalWrite(right, HIGH);
  delay(1000);
  digitalWrite(right, LOW);
  digitalWrite(left, LOW);
  delay(500);
  digitalWrite(right, LOW);
  digitalWrite(left, HIGH);
  delay(1000);
  digitalWrite(right, LOW);
  digitalWrite(left, LOW);
  delay(500);
}
