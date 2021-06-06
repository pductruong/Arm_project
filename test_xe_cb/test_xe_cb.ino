#include <Kalman.h>
#include <Wire.h>
#define Torad PI/180
#define ToDeg 180/PI

Kalman kalman;

#define factortheta PI/20 //Buoc tang cua theta
#define factorphi PI/180  //Buoc tang cua phi

int inChar;
uint32_t timerloop, timerold;

//Khai bao cho dong co
int leftPWM = 9;
int leftDir = 5;
int rightPWM = 10;
int rightDir = 12;

//Khai bao cho encoder
volatile long leftencoder;
volatile long rightencoder;
int leftencoder_a = 2;
int leftencoder_b = 6;
int rightencoder_a = 3;
int rightencoder_b = 7;

//MPU6050 Data
float mpudata;
float accX, accZ;
float gyroY;

uint32_t timer;
uint8_t i2cData[14];

long PWML, PWMR;
const int L_Kp = 1, L_Ki = 0, L_Kd = 0;
const int R_Kp = 1, R_Ki = 0, R_Kd = 0;
const int U_max = 255;


bool mode;// Run = true, Stop = false

int ForwardBack;// 1 -> Forward; -1 -> Back; 0 -> Stop and Balancing
int LeftRight;//1 -> TunrLeft;   -1 -> TurnRight;   0 -> Stop and Balancing

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Set PWM FREQUENCY 31 KHz
  TCCR2B = TCCR2B & B11111000 | B00000001;

  //Output pin control motor left and right
  pinMode(leftPWM, OUTPUT);
  pinMode(leftDir, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(rightDir, OUTPUT);

  //Input pin read encoder
  pinMode(leftencoder_a, INPUT);
  pinMode(leftencoder_b, INPUT);
  pinMode(rightencoder_b, INPUT);
  pinMode(rightencoder_b, INPUT);

  //Interrupt encoder
  attachInterrupt(0, left_isr, RISING);
  attachInterrupt(1, right_isr, RISING);

  //Data MPU6050
  Wire.begin();
  TWBR = ((F_CPU / 400000UL) - 16) / 2;
  i2cData[0] = 7;
  i2cData[1] = 0x00;
  i2cData[2] = 0x00;
  i2cData[3] = 0x00;
  while (i2cWrite(0x19, i2cData, 4, false));
  while (i2cWrite(0x6B, 0x01, true));
  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68){
    Serial.print(F("Error reading sensor"));
    while (1);
  }
  delay(100);
  while (i2cRead(0x3B, i2cData, 6));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  kalman.setAngle(pitch);
  timer = micros();
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Interrupt function left encoder
void left_isr(){
  if(digitalRead(leftencoder_b)){
    leftencoder++;
  }
  else{
    leftencoder--;
  }
}

//Interrupt function right encoder
void right_isr(){
  if(digitalRead(rightencoder_b)){
    rightencoder++;
  }
  else{
    rightencoder--;
  }
}

//Read MPU
void readMPU(){
  while (i2cRead(0x3B, i2cData, 14));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
  double dt = (double)(micros() - timer) / 1000000;
  timer = micros();
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  double gyroYrate = gyroY / 131.0;
  mpudata = kalman.getAngle(pitch, gyroYrate, dt);
  if (abs(mpudata) > 30){
    mode = true;
  }
}
