#include <Servo.h>

// Ultrasonic Sensor Pins
const int trigPin = 13;
const int echoPin = 12;

// Motor Driver Pins
const int frontLeftForward = 4;
const int frontLeftBackward = 5;
const int frontRightForward = 6;
const int frontRightBackward = 7;
const int rearLeftForward = 8;
const int rearLeftBackward = 9;
const int rearRightForward = 10;
const int rearRightBackward = 3;

// Servo Configuration
Servo myServo;
const int servoPin = 11;

// Distance Variables
long duration;
int distance;
int rightDistance, leftDistance;

// Modes
bool automaticMode = true;
char rcCommand = 'S';

// RC Safety Control
unsigned long lastRCCommandTime = 0;
const unsigned long rcTimeout = 300;

void setup() {
  pinMode(frontLeftForward, OUTPUT);
  pinMode(frontLeftBackward, OUTPUT);
  pinMode(frontRightForward, OUTPUT);
  pinMode(frontRightBackward, OUTPUT);
  pinMode(rearLeftForward, OUTPUT);
  pinMode(rearLeftBackward, OUTPUT);
  pinMode(rearRightForward, OUTPUT);
  pinMode(rearRightBackward, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(90);

  Serial.begin(9600);
  delay(1000);
}

void loop() {
  readSerialCommands();

  if (automaticMode) {
    runAutomaticMode();
  } else {
    runRCMode();
  }
}

void readSerialCommands() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n' || c == '\r' || c == ' ') {
      continue;
    }

    if (c == 'M') {
      automaticMode = true;
      rcCommand = 'S';
      stopCar();
      myServo.write(90);
    }
    else if (c == 'R') {
      automaticMode = false;
      rcCommand = 'S';
      stopCar();
      lastRCCommandTime = millis();
    }
    else if (!automaticMode) {
      rcCommand = c;
      lastRCCommandTime = millis();
    }
  }
}

void runAutomaticMode() {
  int frontDistance = readDistance();

  if (frontDistance <= 18) {
    stopCar();
    delay(500);
    chooseDirection();
  } else {
    moveForward();
  }

  delay(50);
}

void runRCMode() {
  if (millis() - lastRCCommandTime > rcTimeout) {
    stopCar();
    return;
  }

  if (rcCommand == 'F') {
    moveForward();
  }
  else if (rcCommand == 'T') {
    moveBackward();
  }
  else if (rcCommand == 'L') {
    turnLeft();
  }
  else if (rcCommand == 'D') {
    turnRight();
  }
  else {
    stopCar();
  }
}

int readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) {
    return 999;
  }

  distance = duration * 0.034 / 2;
  return distance;
}

void chooseDirection() {
  myServo.write(0);
  delay(800);
  rightDistance = readDistance();

  myServo.write(180);
  delay(800);
  leftDistance = readDistance();

  myServo.write(90);
  delay(500);

  if (rightDistance > leftDistance) {
    turnRight();
    delay(650);
  } else {
    turnLeft();
    delay(650);
  }

  stopCar();
}

void moveForward() {
  digitalWrite(frontLeftForward, HIGH);
  digitalWrite(frontRightForward, HIGH);
  digitalWrite(frontLeftBackward, LOW);
  digitalWrite(frontRightBackward, LOW);
  digitalWrite(rearLeftForward, HIGH);
  digitalWrite(rearRightForward, HIGH);
  digitalWrite(rearLeftBackward, LOW);
  digitalWrite(rearRightBackward, LOW);
}

void moveBackward() {
  digitalWrite(frontLeftForward, LOW);
  digitalWrite(frontRightForward, LOW);
  digitalWrite(frontLeftBackward, HIGH);
  digitalWrite(frontRightBackward, HIGH);
  digitalWrite(rearLeftForward, LOW);
  digitalWrite(rearRightForward, LOW);
  digitalWrite(rearLeftBackward, HIGH);
  digitalWrite(rearRightBackward, HIGH);
}

void stopCar() {
  digitalWrite(frontLeftForward, LOW);
  digitalWrite(frontRightForward, LOW);
  digitalWrite(frontLeftBackward, LOW);
  digitalWrite(frontRightBackward, LOW);
  digitalWrite(rearLeftForward, LOW);
  digitalWrite(rearRightForward, LOW);
  digitalWrite(rearLeftBackward, LOW);
  digitalWrite(rearRightBackward, LOW);
}

void turnRight() {
  digitalWrite(frontLeftForward, HIGH);
  digitalWrite(frontRightForward, LOW);
  digitalWrite(frontLeftBackward, LOW);
  digitalWrite(frontRightBackward, HIGH);
  digitalWrite(rearLeftForward, HIGH);
  digitalWrite(rearRightForward, LOW);
  digitalWrite(rearLeftBackward, LOW);
  digitalWrite(rearRightBackward, HIGH);
}

void turnLeft() {
  digitalWrite(frontLeftForward, LOW);
  digitalWrite(frontRightForward, HIGH);
  digitalWrite(frontLeftBackward, HIGH);
  digitalWrite(frontRightBackward, LOW);
  digitalWrite(rearLeftForward, LOW);
  digitalWrite(rearRightForward, HIGH);
  digitalWrite(rearLeftBackward, HIGH);
  digitalWrite(rearRightBackward, LOW);
}