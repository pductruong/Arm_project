#include <MsTimer2.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

int MODE = 1;
double dc1, dc2, dc3, dc4;

const int incPin[] = {4, 5};
const int decPin[] = {6, 7};
const int servoPin[] = {3,2};

String inputString1 = "";
String inputString2 = "";
String inputString3 = "";  
String inputString4 = "";  
boolean stringComplete = false;
short ck = 0;

long positions[2];

AccelStepper stepper1(1,8,9);
AccelStepper stepper2(1,11,12);
MultiStepper steppers;

Servo myservo[2];

void setup() {
  Serial.begin(115200);
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(1000.0);
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(1000.0);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  pinMode(10,OUTPUT); // Enable
  digitalWrite(10,LOW);
  pinMode(13,OUTPUT); // Enable
  digitalWrite(13,LOW);

  for(int a; a < 2; a++)
  {
    pinMode(incPin[a],INPUT);
    pinMode(decPin[a],INPUT);
  }
  myservo[0].attach(servoPin[0]);
  myservo[1].attach(servoPin[1]);
  myservo[0].write(170);
  myservo[1].write(70);
  MsTimer2::set(100, pubData); // 1ms period
  MsTimer2::start();
}

void loop() { 
    if(MODE == 1)
    {
      butStep2();
      butStep1();    
    }
    else
    {
      if(stringComplete == true)
      {
        dc1 = inputString1.toInt();
        dc2 = inputString2.toInt();
        dc3 = inputString3.toInt();
        dc4 = inputString4.toInt();
        positions[0] = long((dc1*6400L*3L)/(360L));
        positions[1] = long((dc2*6400L*3L)/(360L));
        run_stepper(positions);
//        run_servo(dc3);
        myservo[0].write(dc3);
        delay(100);
        myservo[1].write(dc4);
        delay(100);
        Serial.println('D');
        inputString1 = "";
        inputString2 = "";
        inputString3 = "";
        inputString4 = "";
        stringComplete = false;
      }
    }
}

void butStep1()
{
  if(digitalRead(incPin[0]) == 1)
  {
    dc1++;
    stepper1.runToNewPosition((dc1*6400L*3L)/(360L));
  }
  if(digitalRead(decPin[0]) == 1)
  {
    dc1--;
    stepper1.runToNewPosition((dc1*6400L*3L)/(360L));
  }
}

void butStep2()
{
  if(digitalRead(incPin[1]) == 1)
  {
    dc2++;
    stepper2.runToNewPosition((dc2*6400L*3L)/(360L));
  }
  if(digitalRead(decPin[1]) == 1)
  {
    dc2--;
    stepper2.runToNewPosition((dc2*6400L*3L)/(360L));
  }
}

void run_stepper(long positions)
{
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
}

//void run_servo(int angle)
//{
//  int k = myservo.read();
//  
//  if(k < angle)
//  {
//    for(int c = k; c <= angle; c++)
//    {
//      myservo.write(c);
//      delay(15);
//    }
//  }
//  else
//  {
//    for (int c = k; c >= angle; c++)
//    {
//      myservo.write(c);
//      delay(15);
//    }
//  }
//}


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
    else if(inChar == 'M')
    {
      MODE = 1;
    }
    else if(inChar == 'A')
    {
      MODE = 2;
    }
    else
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

void pubData()
{
  Serial.print(dc1);
  Serial.print(",");
  Serial.print(dc2);
  Serial.print(",");
  Serial.print(dc3);
  Serial.print(",");
  Serial.println(dc4);
}
