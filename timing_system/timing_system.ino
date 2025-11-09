#include <Servo.h>

#define LEDR 22
#define LEDG 23
#define LEDB 24
#define SENSOR_PIN 2   // Digital pin connected to your IR receiver output
#define MOTOR_PIN 3

const int CLOSED_ANGLE = 90;   // Closed position
const int OPEN_ANGLE = 0;    // Open position

Servo gateServo;

void setup() {

  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(SENSOR_PIN, INPUT); 
  pinMode(MOTOR_PIN, OUTPUT);

  // Start with green (beam unbroken)
  digitalWrite(LEDR, HIGH);  // OFF
  digitalWrite(LEDG, LOW);   // ON
  digitalWrite(LEDB, HIGH);  // OFF

  gateServo.attach(MOTOR_PIN);
  gateServo.write(CLOSED_ANGLE);


  Serial.println("Setup complete");
}

void loop() {
  int beamState = digitalRead(SENSOR_PIN);
  
  if (beamState == HIGH) {
    // Beam unbroken: green
    Serial.println("Beam unbroken");
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
    
  } else {
    // Beam broken: red
    Serial.println("Beam broken");
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  }


}
