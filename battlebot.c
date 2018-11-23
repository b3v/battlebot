
#include <Servo.h>
#include <RedBot.h>
// Provides special objects, methods, and functions for the RedBot.

RedBotMotors motors; // Instantiate the motor control object. This only needs
// to be done once.

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

// Pins
const int trigPin = 3;
const int echoPin = 11;

// Anything over 400 cm (23200 ms pulse) is "out of range"
const unsigned int maxDist = 23200;

void setup() {
  // attaches the servo on pin 9 to the servo object
  myservo.attach(9);

  // The Trigger pin will tell the sensor to range find
  // The sensor attached to the trigger is an output device
  pinMode(trigPin, OUTPUT);

  // don't turn it on to start
  // pin is set to 0 volts
  digitalWrite(trigPin, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void doStrike(int numberOfStrikes) {
  Serial.println("Striking!!");

  for (int i = 0; i < numberOfStrikes; i++) {
    // striking twice
    myservo.write(180);
    delay(350);
    myservo.write(30);
    delay(300);
  }
}

void loop() {

  int numberOfStrikes = 2;
  doStrike(numberOfStrikes);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure how long the echo pin was held high (pulse width)
  int duration = pulseIn(echoPin, HIGH);

  // Calculate distance in inches.
  // The constant is found in the datasheet,
  // and are calculated from the assumed speed
  // of sound in air at sea level (~340 m/s).
  float inches = duration / 148.0;

  // Print out results
  // if it's more than 1.5 seconds
  if (inches >= 24) {
    Serial.println("Turn Back!");
    Serial.println("There are no objects within 24 inches!");
    Serial.println("");
    // The battlebot first reverses in order to not engage with obstacles/other bots
    // NOTE: It takes about 1100 ms to move 2 feet with .drive(255)
    // therefore, about 2200 ms to move 2 ft with .drive(127)
    motors.drive(-127);
    //  2.2 seconds
    delay(2200);
    // clockwise
    // The right wheel rotates backwards
    motors.rightMotor(-100);
    // The left wheel rotates forwards
    motors.leftMotor(-100);
    // This then turns the battlebot to the right
    // Takes 1150 ms to turn 180 degrees with .[right|left]Motor(-100)
    delay(1150);
  }

  else {
    // we see something within 2 feet
    Serial.print("Watch Out! Object is ");
    Serial.print(inches);
    Serial.println(" inches away");
    Serial.println("");

    // backup 1 foot
    motors.drive(-255);
    delay(650);

    // turn 90 degrees counterclockwise
    // The right wheel rotates forwards
    motors.rightMotor(100);
    // The left wheel rotates backwards
    motors.leftMotor(100);
    // The car turns to the left and then continues to run forwards
    delay(600);
  }

  delay(50);
  motors.drive(127);
  delay(1100);
  motors.brake();
}
