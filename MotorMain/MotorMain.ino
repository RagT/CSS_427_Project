#include <SimpleTimer.h>
#include <dht.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define DHT22_PIN 52 //temp and humidity sensor pin

const int RIGHT = 1;
const int LEFT = -1;

Servo leftServo;
Servo rightServo;
dht DHT;
SimpleTimer timer;
int timerID;
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
int backLight = 13;

int photoRPin = 15;
int led = 51;
int redLed = 6;
int blueLed = 5;
int lightLevel;
int minLevel;
int maxLevel;
int adjustedLightLevel;

byte power = 150;
const int maxPower = 200;

//Stores sensor readings
float temperature = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(led, OUTPUT); 
//  pinMode(redLed, OUTPUT);
//  pinMode(blueLed, OUTPUT);
  leftServo.attach(24);
  rightServo.attach(26);
//  digitalWrite(redLed, LOW);
//  digitalWrite(blueLed, LOW);
  pinMode(backLight, OUTPUT);          //set pin 13 as output
  analogWrite(backLight, 150);       
  
  lcd.begin(16,2);                    // columns, rows. size of display
  lcd.clear();                        // clear the screen
  lcd.setCursor(0,0);                 // set cursor to column 0, row 0 (first row)
  lcd.print("CSS 427 Project");       // input your text here
  lcd.setCursor(0,1);                 // move cursor down one
  lcd.print("Controlled Car");      //input your text here
  lightLevel = analogRead(photoRPin);  
  minLevel = lightLevel-20;
  maxLevel = lightLevel;
}

void loop(){
  bool msgRecieved = false;
  char msg[2];
  while(!msgRecieved) {
    photocellRead();
    checkLightLevel();    
    while(Serial.available()) {
      Serial.readBytes(msg,2);
      msgRecieved = true;
    }
  }
  
  
  //Handle command
    switch(msg[0]) {
      case 'F':
        break;
      case 'W':
        Serial.println("Forward command for small bot recieved by big bot");
        Serial.println("Sending forward command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'A':
        Serial.println("Turn left command for small bot recieved by big bot");
        Serial.println("Sending turn left command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'S':
        Serial.println("Backward command for small bot recieved by big bot");
        Serial.println("Sending backward command to small bot");
        Serial1.write(msg,2);
        readSerial1();
        break;
      case 'D':
        Serial.println("Turn right command for small bot recieved by big bot");
        Serial.println("Sending turn right command to small bot");
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
//      case '+':
//        if(msg[1] == 1) {
//          Serial.println("Decrease power command recieved by big bot");
//          setPower(power + 50);
//          Serial1.write(msg,2);
//          readSerial1();
//        }
//        break;
//      case '-':
//        if(msg[1] == 1) {
//          Serial.println("Decrease power command recieved by big bot");
//          setPower(power - 50);
//          Serial1.write(msg,2);
//          readSerial1();
//        }
//        break;
      case 't':
        if(msg[1] == 1) {
          //stop servos
          leftServo.writeMicroseconds(1500);
          rightServo.writeMicroseconds(1500);

          //Get sensor data for big bot also displays on lcd
          checkTempandHum();
          
          //Request small bot for sensor data
          Serial1.write(msg,2);

          //Get small bot's sensor data and print to serial monitor
          float smallBotTemp = Serial1.readStringUntil('\n').toFloat();
          float smallBotHum = Serial1.readStringUntil('\n').toFloat();
          Serial.print("Temperature(smallBot) = ");
          Serial.print(smallBotTemp);
          Serial.print("C/");
          Serial.print(fahrenheitTemp(smallBotTemp));
          Serial.println("F");
          Serial.print("Humidity(smallBot) = ");
          Serial.print(smallBotHum);
          Serial.println("%");
        }
        break;
    }
}

void forward() {
  leftServo.writeMicroseconds(1500 - power);
  rightServo.writeMicroseconds(1500 + power); 
  Serial.println("Forward command recieved by big bot.");
}

void backward(){
  leftServo.writeMicroseconds(1500 + power);
  rightServo.writeMicroseconds(1500 - power);  
  Serial.println("Backward command recieved by big bot.");
}

void stop(){
  leftServo.writeMicroseconds(1500);
  rightServo.writeMicroseconds(1500);
  Serial.println("Stop command recieved by big bot");
}

void turn(int direction) {
   leftServo.writeMicroseconds(1500 + (power * direction));
   rightServo.writeMicroseconds(1500 + (power * direction));
   if(direction == LEFT) {
      Serial.println("Turn left command recieved by big bot");
   } else {
      Serial.println("Turn right command recieved by big bot");
   }
 }

void readSerial1(){
  char ch = Serial1.read();
  switch(ch) {
    case 'l':
      Serial.println("Turn left command recieved by small bot");
      break;
    case 'r':
      Serial.println("Turn right command recieved by small bot");
      break;
    case 'b':
      Serial.println("Backward command recieved by small bot");
      break;
    case 'f':
      Serial.println("Forward command recieved by small bot");
      break;  
    case 's':
      Serial.println("Stop command recieved by small bot");
      break;
//    case '+':
//      Serial.println("Increase power command recieved by small bot.");
//      break;
//    case '-':
//      Serial.println("Decrease power command recieved by small bot.");
  }
  Serial.println();
}

void setPower(byte newPower) {
  if(power > 0 && power <= maxPower) {
    power = newPower;
  }
}

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
}

void checkLightLevel() {
  if(adjustedLightLevel <= 45) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

void checkTempandHum() {
  int chk = DHT.read22(DHT22_PIN);
  
  //Reads data if sensor ready
  if(chk == DHTLIB_OK) {
    temperature = DHT.temperature;
    humidity = DHT.humidity;
  }
  showDisplay();
  float fTemp = fahrenheitTemp(temperature);
  Serial.println();
  Serial.print("Temperature(bigBot)= ");
  Serial.print(temperature);
  Serial.print("C/");
  Serial.print(fTemp);
  Serial.println("F");
  Serial.print("Humidity(bigBot) = ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.println();
}

float fahrenheitTemp(float celsius) {
  return (celsius * 1.8) + 32; 
}

//Display
void showDisplay() {
  lcd.display();     
  lcd.setCursor(0,0);
  lcd.print("Temp:    ");
  lcd.print(round(temperature));
  lcd.print("C/");
  lcd.print(round(fahrenheitTemp(temperature)));
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
}



