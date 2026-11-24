// Ultrasonic Sensor HC-SR04 with 3-reading averaging
const int trigPin = 9;    // Trigger pin
const int echoPin = 10;   // Echo pin

void setup() {
  Serial.begin(9600);     // Initialize serial communication
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Get the averaged distance
  float distance1 = getAverageDistance(5);
  
  // Print the result to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");
  
  delay(100); // Short delay between measurements
}

// Function to get a single distance reading
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2; // Convert to cm
  
  return distance;
}

// Function to get average of multiple readingsc
float getAverageDistance(int numReadings) {
  float sum = 0;
  
  for (int i = 0; i < numReadings; i++) {
    sum += getDistance();
    delay(30); // Small delay between readings
  }
  
  return sum / numReadings;
}
