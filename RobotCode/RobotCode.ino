#include <Servo.h>

const int RIGHT = 1;
const int LEFT = -1;

Servo leftServo;
Servo rightServo;

const byte led = LED_BUILTIN;
const byte button = 9;
byte power = 150;
const int maxPower = 200;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT); 
  pinMode(button, INPUT_PULLUP);
  leftServo.attach(5);
  rightServo.attach(6);
}

void loop(){
  bool msgRecieved = false;
  char msg[2];

  while(!msgRecieved) {
    while(Serial.available()) {
      Serial.readBytes(msg,2);
      msgRecieved = true;
    }
  }
  //Handle command
    switch(msg[0]) {
      case 'W':
        if(msg[1] == 1) {
          forward();
        } else {
          stop();
        }
        break;
      case 'A':
        if(msg[1] == 1) {
          turn(LEFT);
        } else {
          stop();
        }
        break;
      case 'S':
        if(msg[1] == 1) {
          backward();
        } else {
          stop();
        } 
        break;
      case 'D':
        if(msg[1] == 1) {
          turn(RIGHT);
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

void blinkLed(){
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}

void forward() {
  leftServo.writeMicroseconds(1500 - power);
  rightServo.writeMicroseconds(1500 + power); 
}

void backward(){
  leftServo.writeMicroseconds(1500 + power);
  rightServo.writeMicroseconds(1500 - power);  
}

void stop(){
  leftServo.writeMicroseconds(1500);
  rightServo.writeMicroseconds(1500);
  delay(200);
}

void turn(int direction) {
   leftServo.writeMicroseconds(1500 + (power * direction));
   rightServo.writeMicroseconds(1500 + (power * direction));
}

void setPower(int newPower) {
  if(power > 0 && power <= maxPower) {
    power = (byte) newPower;
  }
}
//void turn(int direction, int degrees) {
//  if(degrees > 0) {
//   float turnConstant = 7.1 * (200 / power);
//   leftServo.writeMicroseconds(1500 + (power * direction));
//   rightServo.writeMicroseconds(1500 + (power * direction));
//   delay(degrees * turnConstant);
//   stop(); 
//  }
//}


//void processCommand(String command) {
//  int spaceIndex = command.indexOf(' ');
//  String commandType = command.substring(0, spaceIndex);
//  String parameters = command.substring(spaceIndex + 1);
//  int param = parameters.toInt();
//  
//  if(commandType == "forward") {
//    forwardTime(param);
//    //Serial.println("Going forward");  
//  } else if(commandType == "backward") {
//    //Serial.println("Going backward");
//    backwardTime(param);
//  } else if(commandType == "turn") {
//     int secondSpace = parameters.indexOf(" "); 
//     String turnDirection = parameters.substring(0, secondSpace);
//     Serial.println(turnDirection);
//     int turnDegrees = parameters.substring(secondSpace).toInt();
//     Serial.println(turnDegrees);
//     if(turnDirection == "right") {
//      turn(RIGHT, turnDegrees);
//     } else if (turnDirection == "left") {
//      turn(LEFT, turnDegrees);
//     } else {
//      //Serial.println("Invalid command");
//     }
//  } else if(commandType == "setPower") {
//    setPower(param);
//  } else {
//    //Serial.println("Invalid command.");
//  }
//}

