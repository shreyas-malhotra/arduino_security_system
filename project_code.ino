//The code provided in this repository may not be fully functional. It will require modification on your part to achieve proper functionality.

#include <Arduino.h>
#include <Keypad.h>
#include <Servo.h>

int pirPin = 11;
int buzzerPin = 10;
int servoPin = 13;
unsigned long lastMotionTime = 0;
const long buzzerDuration = 1500;
const long cooldownPeriod = 30000;
boolean calibrated = false;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myservo;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(servoPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(pirPin, LOW);
  myservo.attach(servoPin);
  Serial.begin(9600);

  delay(1000);

  Serial.println("Calibrating PIR sensor...");
  delay(5000);
  calibrated = true;
  Serial.println("Calibration complete.");
}

void loop() {
  if (calibrated) {
    int pirState = digitalRead(pirPin);
    unsigned long currentTime = millis();

    if (pirState == HIGH) {
      if ((currentTime - lastMotionTime) > cooldownPeriod) {
        Serial.println("Motion detected!");
        activateBuzzer();
        lastMotionTime = currentTime;

        digitalWrite(pirPin, LOW);
      }
    }
  }

  char key = keypad.getKey();

  if (key != NO_KEY) {
    if (key == '9') {
      Serial.println("Authentication Successful");
      activateServo();
    } else {
      Serial.println("Authentication failed.");
    }
  }
}

void activateBuzzer() {
  tone(buzzerPin, 1000);
  delay(buzzerDuration);
  noTone(buzzerPin);
}

void activateServo() {
  myservo.write(90);
}
