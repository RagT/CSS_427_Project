#include <Servo.h>
#include <dht.h>


#define DHT11_PIN 7

const int RIGHT = 1;
const int LEFT = -1;

float temperature = 0.0;
float humidity = 0.0;

dht DHT;
Servo leftServo;
Servo rightServo;

const byte led = LED_BUILTIN;
const byte button = 9;
byte power = 200;
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
      case 't':
        readSensorData();
        break;
    }
}

void forward() {
  leftServo.writeMicroseconds(1500 - power);
  rightServo.writeMicroseconds(1500 + power); 
  Serial.write('f');
}

void backward(){
  leftServo.writeMicroseconds(1500 + power);
  rightServo.writeMicroseconds(1500 - power);
  Serial.write('b');  
}

void stop(){
  leftServo.writeMicroseconds(1500);
  rightServo.writeMicroseconds(1500);
  Serial.write('s');
}

void turn(int direction) {
   leftServo.writeMicroseconds(1500 + (power * direction));
   rightServo.writeMicroseconds(1500 + (power * direction));
   if(direction == LEFT) {
      Serial.write('l');
   } else {
      Serial.println('r');
   }
}

void setPower(int newPower) {
  if(power > 0 && power <= maxPower) {
    power = (byte) newPower;
  }
}

void readSensorData(){
  //Stop bot since reading can take time
  stop();
  int chk = DHT.read11(DHT11_PIN);
  //Reads data if sensor ready
  if(chk == DHTLIB_OK) {
    temperature = DHT.temperature;
    humidity = DHT.humidity;
  }
  //Sends temperature and humidity readings over bluetooth
  Serial.print("Temperature(smallBot)=");
  Serial.println(temperature);
  Serial.print("Humidity(smallBot)=");
  Serial.println(humidity);
}

