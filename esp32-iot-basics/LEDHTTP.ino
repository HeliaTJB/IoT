
#include <HTTPClient.h>
#include <WiFi.h>
#include <TM1637Display.h>

TM1637Display display(12,13);

HTTPClient http;
WiFiClient client;

String ssID = "iPhone";
String pass = "Taj527784";

int led1 = 0, led2 = 0;

uint8_t y[4];

bool touch1 = false, touch2 = false;

void IRAM_ATTR  isr1(){
    touch1 = true;
}
void IRAM_ATTR  isr2(){
    touch2 = true;
}


void setup() {
  // put your setup code here, to run once:
  // pinMode(12,OUTPUT);
  // pinMode(13,OUTPUT);
  // pinMode(14,INPUT);
  Serial.begin(9600);

  display.setBrightness(7);

  WiFi.begin(ssID,pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  http.begin("https://azarakhshmadar.ir/iot_course_sbu/projects/7Segment/esp.php?username=403105063");
  int httpCode = http.GET();
  String payload;
  if(httpCode == 200){    /////means everything is alright -> code 200  , ERRORs can be ocde 404,405,...
    payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.println("ERROR connecting to server!");
  }


  

  //display.setSegments(y);
  // display.showNumberDec(4444);
  // delay(1000);
  // display.clear();

  display.showNumberDec(payload.toInt());
  delay(1000);
  display.clear();

  // if(led1 != int(payload[0]) - 48){
  //   if(int(payload[0] - 48) == 1){
  //     digitalWrite(12,HIGH);
  //     led1 = 1;
  //   }
  //   else {
  //     digitalWrite(12,LOW);
  //     led1 = 0;
  //   }
  // }
  // if(led2 != int(payload[1]) - 48){
  //   if(int(payload[1] - 48) == 1){
  //     digitalWrite(13,HIGH);
  //     led2 = 1;
  //   }
  //   else {
  //     digitalWrite(13,LOW);
  //     led2 = 0;
  //   }
  // }



}




