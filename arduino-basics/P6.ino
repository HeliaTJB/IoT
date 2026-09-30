void setup() {
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
}

void loop() {
  if(digitalRead(13)==0 && digitalRead(12)==0){
    digitalWrite(4,LOW);
    digitalWrite(2,LOW);
  }
  else if(digitalRead(12)==0){
    digitalWrite(4,HIGH);
    digitalWrite(2,HIGH);
    delay(500);
    digitalWrite(4,LOW);
    digitalWrite(2,LOW);
    delay(500);
  }
  else if(digitalRead(13)==0){
    digitalWrite(4,HIGH);
    digitalWrite(2,LOW);
    delay(500);
    digitalWrite(4,LOW);
    digitalWrite(2,HIGH);
    delay(500);
  }
  else {
    digitalWrite(4,HIGH);
    digitalWrite(2,HIGH);
  }
}
