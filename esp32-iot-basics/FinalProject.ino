#include <HTTPClient.h>
#include <WiFi.h>
#include <Bonezegei_DHT11.h>
#include <TM1637Display.h>

HTTPClient http;
WiFiClient client;
Bonezegei_DHT11 dht11(18);
TM1637Display seg(19,21);

String ssID = "iPhone";
String pass = "Taj527784";
String scolor[3] = {"", "", ""};

int led1 = 0, led2 = 0;
bool touch1 = false, touch2 = false;

void IRAM_ATTR  isr1(){
    touch1 = true;
}
void IRAM_ATTR  isr2(){
    touch2 = true;
}

void t1(){
  touch1 = false;
  if(led1 == 0){
      led1 = 1;
      digitalWrite(12,HIGH);
      http.begin(client,"http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063&input=12");
      int httpCode = http.GET();
      if(httpCode == 200)
        Serial.println("request send");
      else 
        Serial.println("ERROR connecting to the server");
    }
    else {
      led1 = 0;
      digitalWrite(12,LOW);
      http.begin(client,"http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063&input=02");
      int httpCode = http.GET();
      if(httpCode == 200)
        Serial.println("request send");
      else 
        Serial.println("ERROR connecting to the server");
    }
}

void t2(){
  touch2 = false;
  if(led2 == 0){
      led2 = 1;
      digitalWrite(13,HIGH);
      http.begin(client,"http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063&input=21");
      int httpCode = http.GET();
      if(httpCode == 200)
        Serial.println("request send");
      else 
        Serial.println("ERROR connecting to the server");
    }
    else {
      led2 = 0;
      digitalWrite(13,LOW);
      http.begin(client,"http://azarakhshmadar.ir/iot_course_sbu/projects/LED/esp.php?username=403105063&input=20");
      int httpCode = http.GET();
      if(httpCode == 200)
        Serial.println("request send");
      else 
        Serial.println("ERROR connecting to the server");
    }
}

int r[3], g[3], b[3];

void setup() {
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,INPUT);
  pinMode(27,INPUT);

  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(32,INPUT);

  dht11.begin();
  Serial.begin(9600);
  seg.setBrightness(7);
  WiFi.begin(ssID,pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  attachInterrupt(14,isr1,FALLING);
  attachInterrupt(27,isr2,FALLING);


  String color[3][3] = {};
  int j = 0;
  while(j<3){
    
  http.begin("http://azarakhshmadar.ir/iot_course_sbu/projects/RGB/esp.php?username=403105063");
    int httpCode = http.GET();
    String payload;
    if(httpCode == HTTP_CODE_OK){
        payload = http.getString();
        Serial.println(payload);
    }
    else {
      Serial.println("EEROR!"+httpCode);
    }

    if(payload!=scolor[j-1] || scolor[0]==""){
    int temp = 0;
    for(int i=0; i<payload.length(); i++){
      if(int(payload[i]) != 45)
        color[temp][j] += payload[i];
        else 
        temp ++;
    }
    Serial.println(color[0][j] + " " + color[1][j] + " " + color[2][j]);

  
    r[j] = color[0][j].toInt();
    g[j] = color[1][j].toInt();
    b[j] = color[2][j].toInt();

    scolor[j] = payload;
    j++;
    }
    }

}


float tem;
int hum;

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
   if(led1 != int(payload[0]) - 48){
    if(int(payload[0]) - 48 == 1){
      digitalWrite(12,HIGH);
      led1 = 1;
    }
    else {
      digitalWrite(12,LOW);
      led1 = 0;
    }
  }
  if(touch1)
    t1();
  if(led2 != int(payload[1]) - 48){
    if(int(payload[1]) - 48 == 1){
      digitalWrite(13,HIGH);
      led2 = 1;
    }
    else {
      digitalWrite(13,LOW);
      led2 = 0;
    }
  }
  if(touch2)
    t2();


  if(led1){
      if (dht11.getData()) {  
    tem = dht11.getTemperature();  
    hum = dht11.getHumidity();
    Serial.print("temperature : ");
    Serial.println(tem); 
    Serial.print("humidity : ");
    Serial.println(hum); 
    } 
    http.begin("http://azarakhshmadar.ir/iot_course_sbu/projects/T&H/esp.php?username=403105063&h="+ String(hum)+"&t="+ String(tem));
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK){
      Serial.println("OK!");
    }
    else {
      Serial.println("ERROR!"+ httpCode);
    }
    
    seg.showNumberDec(hum);
  }
  if(!led1){
    seg.clear();
  }

  if(led2){
    Serial.println(analogRead(32));
      if(analogRead(32)<500){
    analogWrite(26,r[0]);
    analogWrite(25,b[0]);
    analogWrite(33,g[0]);
  }
  else if(analogRead(32)<2000){
    analogWrite(26,r[2]);
    analogWrite(25,b[2]);
    analogWrite(33,g[2]);
  }
  else{
    analogWrite(26,r[1]);
    analogWrite(25,b[1]);
    analogWrite(33,g[1]);
  }
  }
  else if(!led2) {
    analogWrite(26,0);
    analogWrite(25,0);
    analogWrite(33,0);
  }
}




