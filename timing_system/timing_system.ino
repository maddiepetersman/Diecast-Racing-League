#include <Servo.h>

#define LEDR 22
#define LEDG 23
#define LEDB 24
#define SENSOR_PIN 2   // IR receiver output
#define SERVO_PIN 3    // Servo signal pin

const int CLOSED_ANGLE = 90;   // Closed position
const int OPEN_ANGLE = 0;      // Open position

Servo gateServo;

bool raceActive = false;
unsigned long startTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(CLOSED_ANGLE);

  // Start with green (beam unbroken)
  digitalWrite(LEDR, HIGH);  // off
  digitalWrite(LEDG, LOW);   // on
  digitalWrite(LEDB, HIGH);  // off

  Serial.println("Setup complete. Waiting for command...");
}

void loop() {
  // --- 1. Check for Serial commands ---
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("RELEASE")) {
      releaseCar();
    }
  }

  // --- 2. If race is active, monitor the finish line sensor ---
  if (raceActive) {
    int beamState = digitalRead(SENSOR_PIN);

    if (beamState == LOW) { // Beam broken
      unsigned long finishTime = millis();
      float elapsedSeconds = (finishTime - startTime) / 1000.0;

      Serial.print("Finish detected! Time: ");
      Serial.print(elapsedSeconds, 3);
      Serial.println(" seconds");

      raceActive = false;

      // Turn LED red
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);

      // Close the gate again
      delay(1000);
      gateServo.write(CLOSED_ANGLE);
    }
  }
}

// --- Helper function to release car ---
void releaseCar() {
  Serial.println("Releasing car...");
  gateServo.write(OPEN_ANGLE);

  // Turn LED blue to indicate release
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);

  // Give it a short moment before starting timer
  // delay(500);
  startTime = millis();
  raceActive = true;

  Serial.println("Timer started!");
}
