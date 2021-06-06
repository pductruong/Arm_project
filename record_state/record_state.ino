#include <Servo.h>  
#include <AccelStepper.h>

const int servoPin[] = {1, 2};//Servo pin
const int encPin[] = {10,11};//Enable pin
const int incPin[] = {5, 4};//Incremental
const int decPin[] = {6, 7};//Decremantal
int MODE = 1;//Mode 1 - Munual; Mode 2 - Auto

//Interrupt Serial Event
String inputString1 = "";
String inputString2 = "";
String inputString3 = "";  
String inputString4 = "";  

//Mark string when string complete
boolean stringComplete = false;
short ck = 0;//Varible to caculate inputString
volatile double dc1, dc2, dc3, dc4;//Goc dong co
int conti = 0;
int last_state[4] = {0,0,0,0};

AccelStepper stepper(1,8,9);
Servo myservo[2];

void setup() {
  Serial.begin(115200);
  //Config Pin stepper motor
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(1000.0);
  for(int a = 0; a < 2; a++)
  {
    pinMode(enPin[a],OUTPUT);
    digitalWrite(enPin[a],LOW);
    pinMode(incPin[a],INPUT);
    pinMode(decPin[a],INPUT);
  }
  //Config Pin servo motor
  for(int b = 0; b < 2; b++){
    myservo[b].attach(servoPin[b]);
  }
  myservo[0].write(180);
  myservo[1].write(60);
  attachInterrupt(0, modeM, HIGH);
  attachInterrupt(1, modeA, HIGH);
  last_state[2] = myservo[0].read();
  last_state[3] = myservo[1].read();
}

void loop() {
  if(MODE == 1)
  {
    butStep1();
    butStep2();
    butStep4();
    butStep4();
    Serial.print(last_state[0]);
    Serial.print(",");
    Serial.print(last_state[1]);
    Serial.print(",");
    Serial.print(last_state[2]);
    Serial.print(",");
    Serial.println(last_state[3]);
  }

  else if(MODE == 2)
  {
    if(stringComplete == true)
    {
      dc1 = inputString1.toInt();
      dc2 = inputString2.toInt();
      dc3 = inputString3.toInt();
      dc4 = inputString4.toInt();  
      run_stepper(last_state[0],dc1,7,9,0);
      run_stepper(last_state[1],dc2,8,10,1);
      run_servo(last_state[2],dc3,0);
      run_servo(last_state[3],dc3,1);
      Serial.print("D");      
      inputString1 = "";
      inputString2 = "";
      inputString3 = "";
      inputString4 = "";
      stringComplete = false;
    }
  }
}


//Cho nay can doi ra xung hoac ra do. Thu thuc te de tim phuong phap hop li
//Doc nut tang giam cua dong co 1
void butStep1()
{
  if(digitalRead(incPin[0]) == 1)
  {
    digitalWrite(dirPin[0],LOW);
    digitalWrite(stepPin[0],HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin[0],LOW);
    delayMicroseconds(500);
    last_state[0] = last_state[0] +1;
  }
  else if(digitalRead(decPin[0]) == 1)
  {
    digitalWrite(dirPin[0],HIGH);
    digitalWrite(stepPin[0],HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin[0],LOW);
    delayMicroseconds(500);
    last_state[0] = last_state[0] - 1;
  }
  else
  {
    last_state[0] = last_state[0];
  }
}

///Doc nut tang giam cua dong co 2
void butStep2(){
  if(digitalRead(incPin[1]) == 1){
    digitalWrite(dirPin[0],LOW);
    digitalWrite(stepPin[0],HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin[0],LOW);
    delayMicroseconds(500);
    last_state[1] = last_state[1] + 1;
  }
  else if(digitalRead(decPin[1]) == 1){
    digitalWrite(dirPin[0],HIGH);
    digitalWrite(stepPin[0],HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin[0],LOW);
    delayMicroseconds(500);
    last_state[1] = last_state[1] - 1;
  }
  else
  {
    last_state[1] = last_state[1];
  }
}

//Doc nut tang giam cua dong co 3
void butStep3(){
  if(digitalRead(incPin[2]) == 1){
    int ang_servo1 = myservo[0].read() + 1;
    myservo[0].write(ang_servo1);
    last_state[2] = ang_servo1;
  }
  else if(digitalRead(decPin[2]) == 1){
    int ang_servo1 = myservo[0].read() + 1;
    myservo[0].write(ang_servo1);
    last_state[2] = ang_servo1;
  }
  else
  {
    last_state[2] = myservo[0].read();
  }
}

//Doc nut tang giam cua dong co 4
void butStep4(){
  if(digitalRead(incPin[2]) == 1){
    
    last_state[2] = last_state[2] + 1;
  }
  else if(digitalRead(decPin[1]) == 1){
    digitalWrite(dirPin[0],HIGH);
    digitalWrite(stepPin[0],HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin[0],LOW);
    delayMicroseconds(500);
    last_state[1] = last_state[1] - 1;
  }
  else
  {
    last_state[1] = last_state[1];
  }
}

//Che do dieu khien bang tay
void modeM(){
  MODE = 1;
}

//Che do chay tu dong
void modeA(){
  MODE = 2;
}

void run_stepper(int last_state_, int list_state_, int dirPin_, int stepPin_, int save)
{
  if(last_state_ <= list_state_)
  {
    digitalWrite(dirPin_,LOW);
    int b = (((list_state_-last_state_)*6400L*3L)/(360L));
    for (int k = 0; k < b; k++)
    {
      digitalWrite(stepPin_,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin_,LOW);
      delayMicroseconds(500);
    }
  }
  else
  {
    digitalWrite(dirPin_,HIGH);
    int b = ((abs(list_state_-last_state_)*6400L*3L)/(360L));
    for (int k = 0; k < b; k++)
    {
      digitalWrite(stepPin_,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin_,LOW);
      delayMicroseconds(500);
    }
  }
  last_state[save] = list_state_;
}


void run_servo(int last_state_, int list_state_, int a)
{
  if(last_state_ < list_state_)
  {
    for(int k = last_state_; k >= list_state_; k--)
    {
      myservo[a].write(list_state_);
      delay(15);       
    }
  }
  else
  {
    for(int k = last_state_; k >= list_state_; k--)
    {
      myservo[a].write(list_state_);
      delay(15);
    }
  }
}
//Nhan tin hieu tu may tinh
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if(inChar == ',')
    {
      ck++;
    }
    else if(inChar == ';')
    {
      ck = 0; 
      stringComplete = true;
       
    }
    else if(isDigit(inChar))
    {
      if(ck == 0)
      {
        inputString1 += inChar;
      }
      else if(ck == 1)
      {
        inputString2 += inChar;
      }
      else if(ck == 2)
      {
        inputString3 += inChar;
      }
      else if(ck == 3)
      {
        inputString4 += inChar;
      }
    }
  }
}
