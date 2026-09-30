void setup() {
  // put your setup code here, to run once:
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(4,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(4));
  if(analogRead(4)==0){
    analogWrite(26,0);
    analogWrite(25,255);
    analogWrite(33,0);
  }
  else{
    analogWrite(26,0);
    analogWrite(25,255);
    analogWrite(33,0);
  }
}
