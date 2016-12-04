#include <Servo.h>

const int RIGHT = 1;
const int LEFT = -1;

Servo leftServo;
Servo rightServo;

int photoRPin = 15;
int led = 33;
int redLed = 52;
int blueLed = 50;
int lightLevel;
int minLevel;
int maxLevel;
int adjustedLightLevel;

//const byte led = LED_BUILTIN;
const byte button = 9;
byte power = 150;
const int maxPower = 200;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
//  pinMode(led, OUTPUT); 
//  pinMode(button, INPUT_PULLUP);
//  pinMode(redLed, OUTPUT);
//  pinMode(blueLed, OUTPUT);
//  pinMode(led, OUTPUT);
  leftServo.attach(24);
  rightServo.attach(26);
//  digitalWrite(redLed, LOW);
//  digitalWrite(blueLed, LOW);
}

void loop(){
  bool msgRecieved = false;
  char msg[2];
  while(!msgRecieved) {
    while(Serial2.available()) {
      Serial2.readBytes(msg,2);
      msgRecieved = true;
    }
  }
  
  
  //Handle command
    switch(msg[0]) {
      case 'F':
//        if (msg[1] == 1) {
//          flash();
//        }
        break;
      case 'W':
        Serial2.println("Forward command for small bot recieved by big bot");
        Serial2.println("Sending forward command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'A':
        Serial2.println("Turn left command for small bot recieved by big bot");
        Serial2.println("Sending turn left command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'S':
        Serial2.println("Backward command for small bot recieved by big bot");
        Serial2.println("Sending backward command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'D':
        Serial2.println("Turn right command for small bot recieved by big bot");
        Serial2.println("Sending turn right command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'l':
        if(msg[1] == 1) {
         turn(LEFT); 
        } else {
          stop();
        }
        break;
      case 'r':
        if(msg[1] == 1) {
          turn(RIGHT);
        } else {
          stop();
        }
        break;
      case 'u':
        if(msg[1] == 1) {
          forward();
        } else {
          stop();
        }
        break;
      case 'd':
        if(msg[1] == 1) {
          backward();
        } else {
          stop();
        }
        break;
      case '+':
        setPower(power + 25);
        break;
      case '-':
        setPower(power - 25);
        break;
    }
}

void forward() {
  leftServo.writeMicroseconds(1500 - power);
  rightServo.writeMicroseconds(1500 + power); 
  Serial2.println("Forward command recieved by big bot.");
}

void backward(){
  leftServo.writeMicroseconds(1500 + power);
  rightServo.writeMicroseconds(1500 - power);  
  Serial2.println("Backward command recieved by big bot.");
}

void stop(){
  leftServo.writeMicroseconds(1500);
  rightServo.writeMicroseconds(1500);
  Serial2.println("Stop command recieved by big bot");
}

void turn(int direction) {
   leftServo.writeMicroseconds(1500 + (power * direction));
   rightServo.writeMicroseconds(1500 + (power * direction));
   if(direction == LEFT) {
      Serial2.println("Turn left command recieved by big bot");
   } else {
      Serial2.println("Turn right command recieved by big bot");
   }
 }

void readSerial1(){
  char ch = Serial1.read();
  switch(ch) {
    case 'l':
      Serial2.println("Turn left command recieved by small bot");
      break;
    case 'r':
      Serial2.println("Turn right command recieved by small bot");
      break;
    case 'b':
      Serial2.println("Backward command recieved by small bot");
      break;
    case 'f':
      Serial2.println("Forward command recieved by small bot");
      break;  
    case 's':
      Serial2.println("Stop command recieved by small bot");
      break;
  }
}

void setPower(int newPower) {
  if(power > 0 && power <= maxPower) {
    power = (byte) newPower;
  }
}

//void flash() {
//  blinkRed();
//  blinkBlue();
//}
//
//
//void blinkRed(){
//  digitalWrite(redLed, HIGH);
//  delay(20);
//  digitalWrite(redLed, LOW);
//  delay(20);
//}
//
//
//void blinkBlue() {
//  digitalWrite(blueLed, HIGH);
//  delay(50);
//  digitalWrite(blueLed, LOW);
//  delay(50);
//}

//LED and light sensor
void photocellRead() {
  lightLevel = analogRead(photoRPin);
  if(minLevel > lightLevel) {
    minLevel = lightLevel;
  }
  if(maxLevel < lightLevel) {
    maxLevel = lightLevel;
  }




  adjustedLightLevel = map(lightLevel, minLevel, maxLevel, 0, 100);




   Serial1.println(adjustedLightLevel);
   //delay(200);
}




void checkLightLevel() {
  if(adjustedLightLevel >= 70) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
