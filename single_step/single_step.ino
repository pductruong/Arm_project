#include <Servo.h>  
#include <AccelStepper.h>
#include <MultiStepper.h>

const int servoPin[] = {3, 2};
const int enPin[] = {10,13};

long positions[2];
AccelStepper stepper1(1,8,9);
AccelStepper stepper2(1,11,12);
MultiStepper steppers;

Servo myservo[2]; 

int list_state[8][4] = {
{0,-210,140,70},
{0,-210,120,70},
{0,-250,120,20},
{0,0,160,20},
{-30,-210,140,20},
{-30,-210,120,20},
{-30,-250,120,70},
{0,0,160,70},
}; 
int last_state[4] = {0,0, 180, 60};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(1000.0);
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(1000.0);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  for(int a = 0; a < 2; a++)
  {
    pinMode(enPin[a],OUTPUT);
    digitalWrite(enPin[a],LOW);
  }

  for(int b = 0; b < 2; b++){
    myservo[b].attach(servoPin[b]);
  }
   myservo[0].write(160);
   myservo[1].write(70);
}

void loop() {
  // put your main code here, to run repeatedly: 
   for (int i = 0; i < 6; i++)
   {
      positions[0] = long((list_state[i][0]*6400L*3L)/(360L));
      positions[1] = long((list_state[i][1]*6400L*3L)/(360L));
      run_stepper(positions);
      myservo[0].write(list_state[i][2]);
      delay(100);
      myservo[1].write(list_state[i][2]);
      delay(100);
  }
}

void run_stepper(long positions)
{
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
}
