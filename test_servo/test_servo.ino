const int stepPin = 6; 
const int dirPin = 3; 
const int enPin = 9;
int list_state[5] = {90,0,90,0,90};
int last_state = 0;
int a;
void setup() {
  Serial.begin(9600);
    pinMode(stepPin,OUTPUT); 
    pinMode(dirPin,OUTPUT);
  
    pinMode(enPin,OUTPUT);
    digitalWrite(enPin,LOW);
}

 void loop() {
   for (int i = 0; i < 5; i++)
   {
     if(last_state <= list_state[i]){
       digitalWrite(dirPin,LOW);
       int b = (((list_state[i]-last_state)*6400L*3L)/(360L));
       Serial.print("last < state ");
       Serial.println(b);
       for (int j = 0; j < b; j++)
       {
         digitalWrite(stepPin,HIGH);
         delayMicroseconds(500);
         digitalWrite(stepPin,LOW);
         delayMicroseconds(500);
       }
     }
     if (last_state > list_state[i]){
       digitalWrite(dirPin,HIGH);
       int b = ((abs(list_state[i]-last_state)*6400L*3L)/(360L));
       Serial.print("last > list ");
       Serial.println(b);
       for (int j = 0; j < b; j++)
       {
         digitalWrite(stepPin,HIGH);
         delayMicroseconds(500);
         digitalWrite(stepPin,LOW);
         delayMicroseconds(500);
       }
     }
     last_state = list_state[i];
   }
   a = a+1;
 }
