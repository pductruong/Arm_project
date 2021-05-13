#include <Servo.h>  

const int servoPin[] = {10, 12};
const int stepPin[] = {5,6}; 
const int dirPin[] = {2,3}; 
const int enPin[] = {8,9};

Servo myservo[3]; 

int list_state[13][4] = {
  {30, 0, 180, 60}, 
  {30, 130, 180, 60}, 
  {30, 130, 180, 60}, 
  {30, 130, 180, 0}, 
  {30, 0, 180, 0}, 
  {30, 0, 180, 0}, 
  {-30, 0, 180, 0}, 
  {-30, 130, 180, 0},
  {-30, 130, 180, 60}, 
  {-30, 0, 180, 60}, 
  {0, 0, 180, 60},
  {0, 0, 180, 60}}; 
int last_state[4] = {0,0, 180, 60};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for(int a = 0; a < 2; a++)
  {
    pinMode(stepPin[a],OUTPUT); 
    pinMode(dirPin[a],OUTPUT);
  
    pinMode(enPin[a],OUTPUT);
    digitalWrite(enPin[a],LOW);
  }

  for(int b = 0; b < 2; b++){
    myservo[b].attach(servoPin[b]);
  }
   myservo[0].write(180);
   myservo[1].write(60);
}

void loop() {
  // put your main code here, to run repeatedly: 
   
   for (int i = 0; i < 13; i++)
   {
    for(int j = 0; j < 4; j++){
      if(j < 2){
        if(last_state[j] <= list_state[i][j]){
          digitalWrite(dirPin[j],LOW);
          int b = (((list_state[i][j]-last_state[j])*6400L*3L)/(360L));
          for (int k = 0; k < b; k++){
            digitalWrite(stepPin[j],HIGH);
            delayMicroseconds(500);
            digitalWrite(stepPin[j],LOW);
            delayMicroseconds(500);
          }
        }
        else{
          digitalWrite(dirPin[j],HIGH);
          int b = ((abs(list_state[i][j]-last_state[j])*6400L*3L)/(360L));
          for (int k = 0; k < b; k++){
            digitalWrite(stepPin[j],HIGH);
            delayMicroseconds(500);
            digitalWrite(stepPin[j],LOW);
            delayMicroseconds(500);
         }
       }
       last_state[j] = list_state[i][j];
      }
      else{
        if(last_state[j] < list_state[i][j]){
          for(int k = last_state[j]; k <= list_state[i][j]; k++){
            myservo[j-2].write(list_state[i][j]);
            delay(15);
          }          
        }
        else{
          for(int k = last_state[j]; k >= list_state[i][j]; k--){
            myservo[j-2].write(list_state[i][j]);
            delay(15);          
        }
      }
      last_state[j] = list_state[i][j];
    }
    Serial.print("Hoan thanh state thu: ");
    Serial.println(i);
  }
}
   myservo[0].write(180);
   myservo[1].write(60);
}
