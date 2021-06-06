#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
AccelStepper stepper1(1,8,9);
AccelStepper stepper2(1,11,12);
MultiStepper steppers;

const int servoPin = 3;
Servo myservo;

long list_state[2][4] = {
  {30, -130, 160, 0}, 
  {0, 0, 140, 180}};
  
void setup()
{
  Serial.begin(9600);
  stepper1.setMaxSpeed(2000);
  stepper2.setMaxSpeed(2000);
  pinMode(10,OUTPUT); // Enable
  digitalWrite(10,LOW);
  pinMode(13,OUTPUT); // Enable
  digitalWrite(13,LOW);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  myservo.attach(servoPin);
  myservo.write(180);
}



void loop()
{
  long positions[2];
  for(int i = 0;i < 2; i++)
  {
    positions[0] = long((list_state[i][0]*6400L*3L)/(360L));
    positions[1] = long((list_state[i][1]*6400L*3L)/(360L));
    run_stepper(positions);
    myservo.write(list_state[i][2]);
    delay(500);
  }
}

void run_stepper(long positions)
{
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
}
