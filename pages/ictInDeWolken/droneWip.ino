// this uses the Arduino servo library included with version 0012 

// caution, this code sweeps the motor up to maximum speed !
// make sure the motor is mounted securily before running.

#include <Servo.h> 
#include <Wire.h>

const int MPU=0x68;
Servo myservo;
int XMax = -30000;
int XMin = 20000;

void arm(){
 // arm the speed controller, modify as necessary for your ESC  
 setSpeed(0); 
 delay(1000); //delay 1 second,  some speed controllers may need longer
}

void setSpeed(int speed){
 // speed is from 0 to 100 where 0 is off and 100 is maximum speed
 //the following maps speed values of 0-100 to angles from 0-180,
 // some speed controllers may need different values, see the ESC instructions
 int angle = map(speed, 0, 100, 39, 180);
 myservo.write(angle);    
}

void setup()
{
  Serial.begin(115200);
  myservo.attach(9);
  arm();  
  
  // Gyro setup
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);


}


void loop()
{
  int16_t AcX;
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    

  if (AcX > XMax){
    XMax += 0.1 * (AcX - XMax);
  }
  if (AcX < XMin){
    XMin -= 0.1 * (XMin - AcX);
  }
  //Serial.print("Gyro X = ");Serial.println(AcX);
  //Serial.print("Gyro max = ");Serial.println(XMax);
  //Serial.print("Gyro min = ");Serial.println(XMin);
  
  delay(10);
  int speed = map(AcX, XMin, XMax, 24, 32);
  setSpeed(speed);
  delay(10);
  Serial.print("Speed = ");Serial.println(speed);
  XMin += round(abs((XMin-AcX)/400));
  XMax -= round(abs((AcX-XMax)/400));
  
//  // sweep up from 0 to to maximum speed in 20 seconds
//  for(speed = 0; speed <= 100; speed += 5) {
//   setSpeed(speed);
//   delay(1000);
//  }
  // sweep back down to 0 speed.
//  for(speed = 95; speed > 0; speed -= 5) {
//   setSpeed(speed);
//   delay(1000);
//  }
//  setSpeed(0);  
//  delay(5000); // stop the motor for 5 seconds
  }
