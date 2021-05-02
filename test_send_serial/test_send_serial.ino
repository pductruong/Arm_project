int a = 945;
int b = 123;
void setup() {
    Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {

    while (1){
      Serial.print('F');
      Serial.print(a);
      Serial.println("");

      Serial.print('S');
      Serial.print(b);
      Serial.println("");
    }
}