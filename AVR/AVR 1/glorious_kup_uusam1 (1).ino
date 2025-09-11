// Alarm Arduino (Receiver) - put this in the Alarm UNO
#include <SoftwareSerial.h>

const int ledPin = 8;
const int buzzerPin = 9;
SoftwareSerial rxSerial(10, 11); // (RX, TX) -> RX on pin 10 receives from sensor's D11

unsigned long lastLedToggle   = 0;
unsigned long lastBuzzToggle  = 0;
bool ledState = LOW;
bool buzzState = LOW;
int currentDistance = 255;

char buf[8];
int idx = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  rxSerial.begin(9600);
  Serial.begin(9600); // optional debug
}

void loop() {
  // --- Non-blocking read of incoming ASCII distance
  while (rxSerial.available()) {
    char c = rxSerial.read();
    if (c >= '0' && c <= '9' && idx < (int)sizeof(buf)-1) {
      buf[idx++] = c;
    } else if (c == '\n' || c == '\r') {
      if (idx > 0) {
        buf[idx] = '\0';
        currentDistance = atoi(buf);
        idx = 0;
        Serial.print("Received distance: "); Serial.println(currentDistance);
      }
    } // ignore other chars
  }

  unsigned long now = millis();

  if (currentDistance <= 50) {
    // LED: toggle every 500 ms
    if (now - lastLedToggle >= 500) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      lastLedToggle = now;
    }

    // Buzzer: 50% duty, on-time = 10 * distance ms
    unsigned long buzzInterval = (unsigned long)currentDistance * 10UL;
    if (buzzInterval < 20) buzzInterval = 20; // prevent extremely small intervals
    if (now - lastBuzzToggle >= buzzInterval) {
      buzzState = !buzzState;
      digitalWrite(buzzerPin, buzzState);
      lastBuzzToggle = now;
    }
  } else {
    // distance > 50 -> ensure outputs OFF and reset timers
    if (ledState) { ledState = LOW; digitalWrite(ledPin, LOW); }
    if (buzzState) { buzzState = LOW; digitalWrite(buzzerPin, LOW); }
    lastLedToggle = now;
    lastBuzzToggle = now;
  }
}
