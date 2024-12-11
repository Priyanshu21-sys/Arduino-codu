// Pin Definitions
const int forceSensorPin = A0; // Connect the force sensor to analog pin A0

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from the force sensor
  int forceValue = analogRead(forceSensorPin);

  // Print the force value to the Serial Monitor
  Serial.print("Force Sensor Value: ");
  Serial.println(forceValue);

  // Optional: You can add a delay to make the output more readable
  delay(500);
}