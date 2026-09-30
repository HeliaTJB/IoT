
#include <WiFi.h>
bool mode = false;

// void IRAM_ATTR con(){
//   mode = !mode;
//   if(mode==true){
//     WiFi.begin("iPhone","Taj527784");
//   }
//   else{
    
//   }
// }

void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  pinMode(4,OUTPUT);
  WiFi.mode(WIFI_STA);
  //attachInterrupt(2,con,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(2)==1){
    mode = !mode;
    if(mode==true){
     WiFi.begin("iPhone","Taj527784");
  }
  else if(mode==false){
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
  }
  }

  if(WiFi.status() == WL_CONNECTED){
      digitalWrite(4,LOW);
  }
  else {
    digitalWrite(4,HIGH);
  }
}

