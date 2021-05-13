#include <Servo.h>

#define r2p 6400/360  //convert deg to pul
#define ratito1 5.0
#define ratito2 5.0

int PUL[2] = {13 , 12};
int DIR[2] = {11, 10};
int EN[2] = {9, 8};
int servoPin[3] = {7, 6, 5}; //3 servo

int list_state[5][5] = {
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5}};               //list state
int home[5] = {0, 0, 0, 0, 0};     //home state
int cur_state[2] = {0, 0};      //current state

bool stringComplete = false;
String step1 = "";              //string to get data step1 from uart
String step2 = "";              //string to get data step1 from uart
short ck = 0;                   //check param to get data from uart
volatile int step1_lastest = 0, step2_lastest = 0;      //convert string to int

Servo servos[2];

void setup()
{
        Serial.begin(9600);
        step1.reserve(50);
        step2.reserve(50);
        for (int i = 0; i < 2; i++)
        {
                pinMode(PUL[i], OUTPUT);
                pinMode(DIR[i], OUTPUT);
                pinMode(EN[i], OUTPUT);
        }
        for (int j = 0; j < 3;  j++)
        {
                servos[j].attach(servoPin[j]);
        }
}

void loop()
{
        if (stringComplete == true)
        {
                step1_lastest = step1.toInt();
                step2_lastest = step2.toInt();
                Serial.println(step1_lastest);
                Serial.println(step2_lastest);
                cur_state[0] = step1_lastest;
                cur_state[1] = step2_lastest;
                while (true)
                {
                        go_home(cur_state, home);
                        fn_go_state(cur_state,list_state);
                }        
        }
        else
        {
                Serial.println('f');
        }
}

//Function get lastest state of robot
void serialEvent()
{
        if (stringComplete == false)
        {
                while (Serial.available())
                {
                        char inChar = (char)Serial.read();
                        // Serial.println(inChar);
                        if(inChar == '$')
                        {
                                ck = 1;
                        }
                        if (inChar == '#')
                        {
                                ck = 2;
                                stringComplete = true;
                        }
                        if (ck == 0 && isDigit(inChar))
                        {
                                step1 += inChar;
                        }
                        if (ck == 1 && isDigit(inChar))
                        {
                                step2 += inChar;
                        }
                }
        }
}

//Function go home state
void go_home(int current_state[2], int home[5])
{
        for (int i = 0; i < 5; i++)
        {
                if (i < 2)
                {
                        if (current_state[i] < home[i])
                        {
                                digitalWrite(DIR[i], LOW);
                        }
                        else
                        {
                                digitalWrite(DIR[i], HIGH);
                        }
                        for (int j = 0; i < ((int)abs(home[i])*r2p); j++)
                        {
                                digitalWrite(PUL[i], HIGH);
                                delayMicroseconds(500);
                                digitalWrite(PUL[i], LOW);
                                delayMicroseconds(500);
                                cur_state[i] = (int)j/r2p;
                                if (j == 0)
                                {
                                        Serial.print('F');
                                        Serial.print(cur_state[i]);
                                        Serial.println("");
                                }
                                else{
                                        Serial.print('S');
                                        Serial.print(cur_state[i]);
                                        Serial.println("");  
                                }
                                
                        }
                }
                else
                {
                        servos[i].write(home[i]);
                }
        }
}
    

//check lai cho cur_state de print