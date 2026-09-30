#include <Bonezegei_DHT11.h>
#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;
WiFiClient client;
Bonezegei_DHT11 dht11(12);
  // use the class



void setup() { 
    Serial.begin(115200);
    dht11.begin(); // run DHT11 
    WiFi.begin("iPhone","Taj527784");
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(500);
    }
    Serial.println("connected to WiFi");

}

float tempDeg;
float tempFar;
int hum;


void loop() { 
    if (dht11.getData()) { // get All data from DHT11 
    tempDeg = dht11.getTemperature(); // return temperature in celsius 
    tempFar = dht11.getTemperature(true); // return temperature in fahrenheit if true celsius of false 
    hum = dht11.getHumidity(); // return humidity 
    Serial.print("temperature : ");
    Serial.println(tempDeg); 
    Serial.print("humidity : ");
    Serial.println(hum); 
    } 


    http.begin("http://azarakhshmadar.ir/iot_course_sbu/projects/T&H/esp.php?username=403105063&h="+String(hum)+"&t="+String(tempDeg));
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK){
      Serial.println("OK!");
    }
    else {
      Serial.println("ERROR!"+ httpCode);
    }



    delay(2000); 
}

