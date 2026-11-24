const float VOLTAGE_DIVIDER_RATIO = (R1 + R2) / R2; // Replace R1 and R2 with the actual resistor values of your voltage divider circuit
const float MAX_BATTERY_VOLTAGE = 4.2; // Replace with your battery's maximum voltage
const float MIN_BATTERY_VOLTAGE = 3.3; // Replace with your battery's minimum voltage

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // Set the ADC resolution to 12 bits (4096 levels)
}

void loop() {
  int rawValue = analogRead(A0); // Replace A0 with the ADC pin you are using for reading the battery voltage

  // Convert the raw ADC reading to the battery voltage
  
  float voltage = (rawValue / 4095.0) * 3.3 * VOLTAGE_DIVIDER_RATIO;

  // Calculate the battery percentage
  int batteryPercentage = map(voltage, MIN_BATTERY_VOLTAGE, MAX_BATTERY_VOLTAGE, 0, 100);
  batteryPercentage = constrain(batteryPercentage, 0, 100); // Ensure the value is within the valid range

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.print("V | Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.println("%");

  delay(5000); // Wait for 5 seconds before taking the next reading
}
