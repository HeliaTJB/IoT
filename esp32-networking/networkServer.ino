
#include<WiFi.h>
#include<AsyncTCP.h>


AsyncClient* client;


void aftercon(void* arg,AsyncClient* client){
  Serial.println("connnected to IP ... ");


}

void afterdata(void* arg,AsyncClient* client,void* data,size_t len){
  Serial.println((char*)data);
}

void setupTCP(){
    client -> onConnect(aftercon);
    client -> onData(afterdata);

    client -> connect("192.168.0.110",2000);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(12,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin("TP-LINK_BC8008","12345678");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("Connnected");
  Serial.println(WiFi.localIP());

  client = new AsyncClient;
  setupTCP();
}

void loop() {
  // put your main code here, to run repeatedly:

}
