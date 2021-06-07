#include <Servo.h>  
#include <AccelStepper.h>
#include <MultiStepper.h>

const int servoPin[] = {3, 2};
const int enPin[] = {10,13};
const int butPin = 4;
int state = 0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

long positions[2];
AccelStepper stepper1(1,8,9);
AccelStepper stepper2(1,11,12);
MultiStepper steppers;

Servo myservo[2]; 

int list_state[16][4] = {
{0,-140,140,70},
{0,-160,120,70},
{0,-210,120,70},
{0,-250,100,70},
{0,-280,100,20},
{0,-250,100,20},
{0,-210,120,20},
{0,-210,120,20},
{0,-150,140,20},
{0,0,160,20},
{-30,-140,140,20},
{-30,-200,110,20},
{-30,-280,100,70},
{-30,-200,110,70},
{-30,-150,140,70},
{0,0,170,70}
}; 
int last_state[4] = {0,0, 180, 60};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  inputString.reserve(200);
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(1000.0);
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(1000.0);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  pinMode(butPin,INPUT);
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
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    state = inputString.toInt();
    inputString = "";
    stringComplete = false;
  }
  if(state == 1){
    for (int i = 0; i < 6; i++)
    {
      positions[0] = long((list_state[i][0]*6400L*3L)/(360L));
      positions[1] = long((list_state[i][1]*6400L*3L)/(360L));
      run_stepper(positions);
      run_servo(list_state[i][2],0);
      run_servo(list_state[i][3],1);
    }
  }
  else
  {
    state = 0;
  }
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void run_stepper(long positions)
{
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
}

void run_servo(int angle, int r)
{
  int k = myservo[r].read();
  
  if(k < angle)
  {
    while (k < angle){
      k++;
      myservo[r].write(k);
      delay(15);
    }
  }
  else
  {
    while (k > angle)
    {
      k--;
      myservo[r].write(k);
      delay(15);
    }
  }
}
