#include <Servo.h>

// Define pins
#define SWITCH_PIN 2       // Toggle switch to change modes
#define POT_PIN A0         // Potentiometer/joystick input for manual control
#define TRIG_PIN 3         // Ultrasonic sensor trigger pin (for automatic mode)
#define ECHO_PIN 4         // Ultrasonic sensor echo pin (for automatic mode)
#define MOTOR_PIN 9        // Motor control pin (via PWM)

// Variables
bool isAutomatic = true;   // Start in automatic mode by default
Servo motor;               // Motor as a servo for this example

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  motor.attach(MOTOR_PIN);
  motor.write(90);         // Stop motor initially (servo neutral position)
  
  Serial.begin(9600);
}

void loop() {
  // Check switch state
  isAutomatic = digitalRead(SWITCH_PIN) == LOW; // LOW = Automatic mode

  if (isAutomatic) {
    automaticMode();
  } else {
    manualMode();
  }
}

// Automatic mode logic
void automaticMode() {
  Serial.println("Automatic Mode");
  
  // Measure distance using ultrasonic sensor
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 20) {
    motor.write(0);   // Stop motor
    Serial.println("Obstacle detected! Stopping.");
  } else {
    motor.write(180); // Move forward
    Serial.println("Moving forward.");
  }

  delay(100);
}

// Manual mode logic
void manualMode() {
  Serial.println("Manual Mode");

  // Read potentiometer value
  int potValue = analogRead(POT_PIN);
  int motorSpeed = map(potValue, 0, 1023, 0, 180); // Map to motor speed range
  motor.write(motorSpeed);

  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(", Motor Speed: ");
  Serial.println(motorSpeed);

  delay(100);
}

// Function to measure distance
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}
