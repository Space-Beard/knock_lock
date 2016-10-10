#include <Servo.h>
Servo myServo;

const int piezo = A0;
const int switchPin = 2;
const int yellowLed = 3;
const int greenLed = 4;
const int redLed = 5;

int knockVal;
int switchVal;

const int quietKnock = 10;
const int loudKnock = 100;

boolean locked = false;
int numberOfKnocks = 0;

void setup() {
  // set up the pin modes etc
  myServo.attach(11);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(switchPin, INPUT);

  Serial.begin(9600);

  digitalWrite(greenLed, HIGH);
  myServo.write(0);
  Serial.println("The box is unlocked!");

}

void loop() {
  // Check if the box is locked or not, if it isn't, checks if the lock
  // button is pressed. If it's locked, will listen for a knock pattern
  if ( locked == false ){
    switchVal = digitalRead(switchPin);

    if ( switchVal == HIGH ){
      locked = true;
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      myServo.write(90);
      Serial.println("The box is locked!");
      delay(1000);
    }
  }

  if ( locked == true){
    knockVal = analogRead(piezo);

    if ( numberOfKnocks < 3 && knockVal > 0 ){
      if ( checkForKnock(knockVal) == true ){
        numberOfKnocks++;
      }
      Serial.print(3-numberOfKnocks);
      Serial.println(" more knocks to go");
    }

    if ( numberOfKnocks >= 3 ){
      locked = false;
      myServo.write(0);
      delay(20);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
      Serial.println("The box is unlocked!");
      numberOfKnocks = 0;
    }
  }


}

boolean checkForKnock (int value){
    if ( value > quietKnock && value < loudKnock ){
      digitalWrite(yellowLed, HIGH);
      delay(20);
      digitalWrite(yellowLed, LOW);
      Serial.print("Valid knock of value ");
      Serial.println(value);

      return true;
    }
    else {
     Serial.print("Bad knock value ");
     Serial.println(value);

      return false;
    }
  }
