int mode = 0;
bool temp = false;

void p(int m){
  if(m==0){
    for(int i=0;i<3;i++){
    digitalWrite(13,HIGH);
    digitalWrite(12,LOW);
    delay(250);
    digitalWrite(13,LOW);
    digitalWrite(12,LOW);
    delay(250);}
  }
  else if(m==1){
    for(int i=0;i<3;i++){
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    delay(250);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    delay(250);}
  }
  else if(m==2){
    for(int i=0;i<3;i++){
    digitalWrite(13,HIGH);
    digitalWrite(12,HIGH);
    delay(250);
    digitalWrite(13,LOW);
    digitalWrite(12,LOW);
    delay(250);}
  }
  else {
    for(int i=0;i<3;i++){
    digitalWrite(13,HIGH);
    digitalWrite(12,LOW);
    delay(250);
    digitalWrite(13,LOW);
    digitalWrite(12,HIGH);
    delay(250);}
  }
  temp = false;
}

void IRAM_ATTR func(){
  mode = (mode+1)%4;
  temp = true;
  //p((mode-1)%4);
}

void IRAM_ATTR func1(){
  temp = true;
  //p((mode-1)%4);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(27,INPUT);
  pinMode(13,OUTPUT);
  pinMode(14,INPUT);
  pinMode(12,OUTPUT);

  Serial.begin(9600);

  attachInterrupt(14,func,FALLING);
  attachInterrupt(27,func1,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);

  Serial.println(mode);

  if(temp)
    p((mode-1)%4);

}


