// Pin definitions
const int sensorPin = A0; // Sensor input (analog)
const int ledPin = 13;     // LED output (digital)
const int buzzerPin = 9;   // Buzzer output (digital)

// Variables
int sensorValue = 0;
long distance;   // Distance value from sensor
long buzzerDuration; // Buzzer duration in milliseconds

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // For debugging purposes
}

void loop() {
  // Read the sensor value (potentiometer value between 0 and 1023)
  sensorValue = analogRead(sensorPin);

  // Map the sensor value to a range of 0 to 100 (assuming a 50% duty cycle)
  distance = map(sensorValue, 0, 1023, 0, 100);

  // If sensor reads less than or equal to 50, perform the required tasks
  if (distance <= 50) {
    // Toggle LED every half second (500ms)
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);

    // Toggle Buzzer for (10 * distance) milliseconds with a 50% duty cycle
    buzzerDuration = 10 * distance;
    long halfCycle = buzzerDuration / 2;
    
    // Start buzzing
    digitalWrite(buzzerPin, HIGH);
    delay(halfCycle);
    
    // Stop buzzing
    digitalWrite(buzzerPin, LOW);
    delay(halfCycle);
  }
  else {
    // Turn off the LED and buzzer if the sensor value is greater than 50
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  
  // Debugging output
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  Serial.print("Mapped Distance: ");
  Serial.println(distance);

  // Delay between readings to avoid flooding the serial output
  delay(100);
}
