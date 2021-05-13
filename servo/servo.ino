#include <Servo.h>     

//int servoPin[3] = {9, 10 , 11}; //joint 5,4,3
//
//Servo servo[3];
Servo servo1;
Servo servo2;
Servo servo3;
//int state[3] = {85, 0, 180};

void setup(){
//  for(int i = 0; i < 3; i++){
//    servo[i].attach(servoPin[i]);
//  }
  servo1.attach(11);
  servo2.attach(10);
  servo3.attach(12);
} 

void loop() {
  for(int i = 0; i <= 60; i++){
    servo3.write(i);
    delay(15);
  }
  for(int i = 60; i >= 0; i--){
    servo3.write(i);
    delay(15);
  }
//    servo2.write(170);
//    servo3.write(60);
//    servo3.write(60);
//    delay(1000);
//    servo3.write(0);
//    delay(1000);
//    servo3.write(60);
//    delay(1000);
    
} 
