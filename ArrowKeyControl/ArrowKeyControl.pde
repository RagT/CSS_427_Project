import processing.serial.*;

byte[] msg = new byte[2];
Serial port;

void setup() {
  println("Arduino Control Program");
  println("Control main bot with arrow keys");
  println("Control smaller bot with wasd");
  
  println(Serial.list());
  
  port = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  readFromSerial();  
}

public void keyPressed() {
  if(key == CODED) {
    switch(keyCode) {
      case LEFT:
        msg[0] = 'l';
        break;
      case RIGHT:
        msg[0] = 'r';
        break;
      case UP:
        msg[0] = 'u';
        break;
      case DOWN:
        msg[0] = 'd';
        break;
    }
  }
  switch(key) {
    case 'w':
      //forward
      msg[0] = 'W';
      break;
    case 'a':
      //left
      msg[0] = 'A';
      break;
    case 's':
      //backward
      msg[0] = 'S';
      break;
    case 'd':
      //right
      msg[0] = 'D';
      break;
    case 'f':
      //flash led's
      msg[0] = 'F';
      break;
    case '=':
      //increase power
      msg[0] = '+';
      break;
    case '-':
      //decrease power
      msg[0] = '-';
      break;
  }
  msg[1] = 1; // 1 for key pressed
  port.write(msg); // send key info over bluetooth
}

public void keyReleased() {
  //stop
  if(key == CODED) {
    switch(keyCode) {
      case LEFT:
        msg[0] = 'l';
        break;
      case RIGHT:
        msg[0] = 'r';
        break;
      case UP:
        msg[0] = 'u';
        break;
      case DOWN:
        msg[0] = 'd';
        break;
    }
  }
  switch(key) {
    case 'w':
      //forward
      msg[0] = 'W';
      break;
    case 'a':
      //left
      msg[0] = 'A';
      break;
    case 's':
      //backward
      msg[0] = 'S';
      break;
    case 'd':
      //right
      msg[0] = 'D';
      break;
  }
  msg[1] = 0; // 0 for key released
  port.write(msg); // send key info over bluetooth
}

public void readFromSerial() {
  String serialString = port.readStringUntil('\n');
  if(serialString != null) {
   println(serialString); 
  }
}