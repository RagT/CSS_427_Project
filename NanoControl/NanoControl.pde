import processing.serial.*;

int x; //steering variable
int y; //power variable
int leftMotor;
int rightMotor;
final int MAX_POWER = 200;
byte[] msg = new byte[2];
Serial port;

void setup(){
  size(400, 400);
  //port = new Serial(this,Serial.list()[0],9600);
}

void draw(){
  background(204);
  
  fill(0,102,153);
  line(0,200,400,200);
  line(200,0,200,400);
  text("x: " + x,10,20);
  text("y: " + y,10,40);
  text("left: " + leftMotor,10,390);
  text("right: " + rightMotor,230,390);
  
  //port.write(msg);
}

void mouseMoved(){
 x = mouseX - 200;
 y = 200 - mouseY;
 
 leftMotor = coerce(y + x);
 rightMotor = coerce(y - x);
 
 msg[0] = (byte) leftMotor;
 msg[1] = (byte) rightMotor;
}

int coerce(int value) {
 if(value > MAX_POWER) {
   return 100;
 }
 if(value < -1 * MAX_POWER) {
   return -100; 
 }
 if(abs(value) < 10) {
   return 0;
 }
   return value;
}