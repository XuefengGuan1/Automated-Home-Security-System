int pin = 12;
int pinBuzzer = 27;
int switchState = 0;
void setup(){
  pinMode(pin,INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);
  Serial.begin(115200);
}
void loop(){
  switchState = digitalRead(pin);

  if(switchState == LOW){
    Serial.println("switch is closed");
    digitalWrite(pinBuzzer,LOW);
  }else{
    Serial.println("switch is open");
    digitalWrite(pinBuzzer,HIGH);
  }
  delay(500);
}