#include <HTTPClient.h>
#include <WiFi.h>

HTTPClient http;
WiFiClient client;

String ssID = "iPhone";
String pass = "Taj527784";

int led = 0;
bool touch = false;

void IRAM_ATTR  isr(){
    touch = true;
}

void t(){
  touch = false;
  if(led == 0){
      led = 1;
      digitalWrite(12,HIGH);
      http.begin(client,"http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063&input=1");
      int httpCode = http.GET();
      if(httpCode == 200)
        Serial.println("request send");
      else 
        Serial.println("ERROR connecting to the server");
    }
    else {
      led = 0;
      digitalWrite(12,LOW);
      http.begin(client,"http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063&input=0");
      int httpCode = http.GET();
      if(httpCode == 200)
        Serial.println("request send");
      else 
        Serial.println("ERROR connecting to the server");
    }
}

void setup() {
  pinMode(12,OUTPUT);
  pinMode(14,INPUT);
  Serial.begin(9600);
  WiFi.begin(ssID,pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  attachInterrupt(14,isr,FALLING);
}

void loop() {
  delay(1000);
  http.begin("http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063");
  int httpCode = http.GET();
  String payload;
  if(httpCode == 200){ 
    payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.println("ERROR connecting to server!");
  }
   if(led != int(payload[0]) - 48){
    if(int(payload[0]) - 48 == 1){
      digitalWrite(12,HIGH);
      led = 1;
    }
    else {
      digitalWrite(12,LOW);
      led = 0;
    }
  }
  if(touch)
    t();
}




