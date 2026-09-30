
// hw_timer_t* Timer0_Cfg;
// int temp = 0;
// int sum = 0;

// void tmp3(bool mode){
//   if(mode){
//     digitalWrite(21,HIGH);
//     delay(100);
//   }
//   else if(!mode){
//     digitalWrite(21,LOW);
//     delay(100);
//   }
// }

// void IRAM_ATTR tmp2(){
//   sum ++;
// }

// void IRAM_ATTR tmp(){
//   //temp = 999;
//   if(temp==0){
//     temp = 1;
//     digitalWrite(4,HIGH);
//   }
//   else{
//     temp = 0;
//     digitalWrite(4,LOW);
//   }
//   timerWrite(Timer0_Cfg,0);
//   timerAlarm(Timer0_Cfg,10000,false,0);
//   //delay(100);
// }

// // void setup(){
// //   pinMode(2,INPUT);
// //   pinMode(4,OUTPUT);
// //   Serial.begin(9600);
// //   attachInterrupt(2,tmp,FALLING);
// // }
// // void loop(){
// //   temp ++;
// //   Serial.println(temp);
// //   digitalWrite(4,LOW);
// //   delay(100);
// // }


// void setup(){
//   Serial.begin(9600);
//   pinMode(4,OUTPUT);
//   pinMode(21,OUTPUT);
//   pinMode(2,INPUT);
//   pinMode(19,INPUT);

//   Timer0_Cfg = timerBegin(10000); ///ferecans HZ
//   timerWrite(Timer0_Cfg,0);
//   timerAttachInterrupt(Timer0_Cfg,&tmp);
//   timerAlarm(Timer0_Cfg,10000,false,0);

//   attachInterrupt(19,tmp2,FALLING);
// }

// bool mode = true;

// void loop (){
//   //temp ++;
//   delay(100);
//   if(digitalRead(2)==1){
//     mode = !mode;
//     sum = 0;
//   }
//   tmp3(mode);
//   Serial.println(sum);
// }


#include <WiFi.h>

void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin("iPhone","Taj527784");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connnected");
  Serial.println(WiFi.localIP());
}

/// IP : 172.20.10.3

void loop (){

}



