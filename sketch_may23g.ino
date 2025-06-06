#include <Servo.h> // Include the Servo library

// Pin and variable declarations
int potPin = A1; // Potentiometer connected to analog pin A1 (0 - 1023)
int leftMotorPin = 13;  // Left motor signal pin
int rightMotorPin = 12; // Right motor signal pin
int frontLeftMotorPin = 11; // Front left motor signal pin
int frontRightMotorPin = 10; // Front right motor signal pin

const int trigPin = 8; // Ultrasonic sensor trigger pin
const int echoPin = 7; // Ultrasonic sensor echo pin
#define BUMP_SENSOR_PIN A2 // Bumper switch pin
#define LIMIT_SWITCH_PIN 9 // Limit switch pin
int trackerPin = A4; // Line sensor pin

Servo leftMotor;       // Left motor
Servo rightMotor;      // Right motor
Servo frontLeftMotor;  // Front left motor
Servo frontRightMotor; // Front right motor

bool isBumperPressed = false; // Bumper switch state
bool isLimitSwitchPressed = false; // Limit switch state
int lineVal = 0; // Line sensor value

void setup() {
  Serial.begin(9600); // Start serial communication

  // Attach all motors to their respective pins
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);
  frontLeftMotor.attach(frontLeftMotorPin);
  frontRightMotor.attach(frontRightMotorPin);

  // Initialize sensors
  pinMode(trigPin, OUTPUT); // Set trigger pin as output
  pinMode(echoPin, INPUT);  // Set echo pin as input
  pinMode(BUMP_SENSOR_PIN, INPUT_PULLUP); // Set bumper switch pin as input with pull-up resistor
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP); // Set limit switch pin as input with pull-up resistor
  pinMode(trackerPin, INPUT); // Set line sensor pin as input
}

void loop() {
  // Potentiometer logic
  int potValue = analogRead(potPin); // Read the potentiometer value (0-1023)
  int motorSpeed = map(potValue, 0, 1023, 1200, 2000); // Map it to motor speed range (1200-2000)

  // Ultrasonic sensor logic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Measure the duration of the echo pulse
  long distance = duration * 0.034 / 2;   // Calculate the distance in centimeters

  // Bumper switch logic
  isBumperPressed = (digitalRead(BUMP_SENSOR_PIN) == LOW); // Check if bumper is pressed (LOW = pressed)

  // Limit switch logic
  isLimitSwitchPressed = (digitalRead(LIMIT_SWITCH_PIN) == LOW); // Check if limit switch is pressed (LOW = pressed)

  // Line sensor logic
  lineVal = analogRead(trackerPin); // Read the line sensor value

  // Debugging: Print sensor states
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Bumper: ");
  Serial.print(isBumperPressed ? "Pressed" : "Not Pressed");
  Serial.print(", Limit Switch: ");
  Serial.print(isLimitSwitchPressed ? "Pressed" : "Not Pressed");
  Serial.print(", Line Tracker Value: ");
  Serial.println(lineVal);

  // Stop motors if any condition is met
  if (isBumperPressed || isLimitSwitchPressed || (distance > 0 && distance <= 60) || lineVal > 500) {
    leftMotor.writeMicroseconds(1500); // Stop left motor
    rightMotor.writeMicroseconds(1500); // Stop right motor
    frontLeftMotor.writeMicroseconds(1500); // Stop front left motor
    frontRightMotor.writeMicroseconds(1500); // Stop front right motor
    Serial.println("Motors stopped due to sensor condition.");
  } else {
    // Set all motors to the calculated speed
    leftMotor.writeMicroseconds(motorSpeed);
    rightMotor.writeMicroseconds(motorSpeed);
    frontLeftMotor.writeMicroseconds(motorSpeed);
    frontRightMotor.writeMicroseconds(motorSpeed);
  }

  delay(100); // Small delay for stability
}