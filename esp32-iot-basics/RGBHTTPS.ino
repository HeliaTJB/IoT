
#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;
WiFiClient client;



void setup() { 
    Serial.begin(115200);
    pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
    pinMode(14,OUTPUT);
    WiFi.begin("iPhone","Taj527784");
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(500);
    }
    Serial.println("connected to WiFi");

}



void loop() { 
   delay(1000);

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


    String color[3] = {};
    int temp = 0;
    for(int i=0; i<payload.length(); i++){
      if(int(payload[i]) != 45)
        color[temp] += payload[i];
        else 
        temp ++;
    }
    Serial.println(color[0] + " " + color[1] + " " + color[2]);

    int r, g, b;
    r = color[0].toInt();
    g = color[1].toInt();
    b = color[2].toInt();

    analogWrite(13,r);
    analogWrite(12,b);
    analogWrite(14,g);
 
}

