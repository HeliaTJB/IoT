#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>            
#include <ArduinoJson.h>    
#include <ESP8266WiFi.h>
#include <TM1637Display.h>
#include <Base64.h>


base64 Base64;
HTTPClient https;
std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);


const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";



BearSSL::X509List cert(root_ca);
String JsonData;
char Relays[5] = "0000";   
StaticJsonDocument<1024> doc;
String password;
String ssid;
String PCB_SN;
String PCB_PW;
String Online_Mode;
String Mode;
String Pass;
String Address_Remote[5];
String Mode_Realy[4];
int Time_Relay_Start[4][2];
int Time_Relay_Stop[4][2];
int Number_Remotes;
String WIFITEMP;
TM1637Display display(13, 12);



int PinOfRelays[4] = {0,4,5,3};
int PinOfBottoms[4] = { , , , };
String payload;
String lastState = "0000";
String lastBottomStates = "0000";
String currentBottomStates = "0000";



void GetInfo(){

  if (!LittleFS.begin()) {
    Serial.println("LittleFS Failed");
    return;
  }
  File file = LittleFS.open("/info.json", "r");
  if (!file) {
    Serial.println("Failed to open json");
    return;
  }
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.print("Failed of JSON: ");
    Serial.println(error.c_str());
    return;
  }

  ssid = doc["wifi_ssid"].as<String>();
  password = doc["wifi_password"].as<String>();
  PCB_SN = doc["PCB_SN"].as<String>();
  PCB_PW = doc["PCB_PW"].as<String>();
  Online_Mode = doc["Online_Mode"].as<String>();
  Mode = doc["Pass_Man"]["Mode"].as<String>();
  Pass = doc["Pass_Man"]["Pass"].as<String>();
  Number_Remotes = doc["number_remote"]["Number_Of_Remotes"];

  for(int i =0; i< Number_Remotes; ++i) {
  Address_Remote[0] = doc["number_remote"][("Address_remote_"+ String(i+1)).c_str()].as<String>();
  }

  Mode_Realy[0] = doc["Mode_relay"]["Mode_relay_1"].as<String>();
  Mode_Realy[1] = doc["Mode_relay"]["Mode_relay_2"].as<String>();
  Mode_Realy[2] = doc["Mode_relay"]["Mode_relay_3"].as<String>();
  Mode_Realy[3] = doc["Mode_relay"]["Mode_relay_4"].as<String>();

  for (int i = 0; i<4; i++) {
    Time_Relay_Start[i][0] = doc["Time_Relay"][String("Time_Relay_" + String(i+1)).c_str()][0][0];  
    Time_Relay_Start[i][1] = doc["Time_Relay"][String("Time_Relay_" + String(i+1)).c_str()][0][1]; 
    Time_Relay_Stop[i][0] = doc["Time_Relay"][String("Time_Relay_" + String(i+1)).c_str()][1][0];   
    Time_Relay_Stop[i][1] = doc["Time_Relay"][String("Time_Relay_" + String(i+1)).c_str()][1][1];  
  }
 

  file.close();
}

void ChangeJSON(){

  doc["wifi_ssid"] = "iPhone";
  doc["wifi_password"] = "Taj527784";
  doc["PCB_SN"] = "5678";
  doc["PCB_PW"] = "javad1383";


  File file = LittleFS.open("/info.json", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (serializeJsonPretty(doc, file) == 0) {
    Serial.println("Failed to write JSON to file");
  } 
  else {
    Serial.println("JSON written successfully");
  }

  file.close();
}

void Convert_To_Json(String mode, int relay_number, String expectedData){
  if(relay_number == 0) { 
    delay(1); 
  }
  else if (Relays[relay_number - 1] == '0') { 
    Relays[relay_number - 1] = '1'; 
  }
  else {
    Relays[relay_number - 1] = '0';
  }
  JsonData = "{\"Mode\":\"" + mode + "\",\"PCB_SerialNumber\":\"" + PCB_SN + "\",\"PCB_PassWord\":\"" + PCB_PW +  "\",\"RemoteData\":\"" + String(Relays) + "\",\"ExpectedData\":\"" + expectedData + "\"}";
}

void stillConnected(){
  if(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      while (WiFi.status()!= WL_CONNECTED) {
        Serial.println("...");
        delay(1000);
      }
      Serial.println("connected");
  }
}

void WiFiHanddelling(){
  
  GetInfo();

  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED) {
    Serial.println("...");
    Serial.println(ssid +" "+ password);
    delay(1000);
  }
  Serial.println("connected");

  configTime(3.5 * 3600, 0, "pool.ntp.org", "time.google.com"); 
  Serial.print("Waiting for time to be set...");
  while (time(nullptr) < 8 * 3600 * 2) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTime is set.");

  client->setTrustAnchors(&cert);


  if(ssid == "iPhone" && password == "Taj527784")
  {

  https.begin(*client,"https://azarakhsh-test.liara.run/board/situation");
  String credentials = PCB_SN + ":" + PCB_PW;
  String encoded = base64::encode(credentials);
  https.addHeader("Authorization", "Basic " + encoded);


  Convert_To_Json("read", 0, "wifi");

  int code = https.POST(JsonData);
  if (code == 200) {
    WIFITEMP = https.getString();
    Serial.println("Received WiFi info:\n" + WIFITEMP);
  } 
  else {
    Serial.print("Failed to get WiFi info. Code: ");
    Serial.println(code);
    return;
  }
  https.end();

  int index = WIFITEMP.indexOf('\n');
  if (index > 0) {
    ssid = WIFITEMP.substring(0, index);
    password = WIFITEMP.substring(index + 1);
    Serial.println(ssid + " " + password);
  } 
  else {
    Serial.println("format error!");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi from server information");

  ChangeJSON();
  
  }
}

void CheckSituation(){
  for(int i = 0; i < 4; ++i) {
    if(payload[i]!=lastState[i]){
      if(payload[i]=='0'){
        digitalWrite(PinOfRelays[i], LOW);
      }
      else {
        digitalWrite(PinOfRelays[i], HIGH);
      }
      lastState[i] = payload[i];
      }
  
  }

  int temp = payload.toInt();
  temp = temp + 1000;
  display.showNumberDec(temp);
}

// void ChangeSituation(){


//   for(int i=0; i<4; ++i){
//       currentBottomStates[i] = (digitalRead(PinOfBottoms[i])==LOW) ? '1' : '0';

//       if(currentBottomStates[i] != lastBottomStates[i]){



//           https.begin(*client,"https://azarakhsh-test.liara.run/board/mode");

//           String credentials = PCB_SN + ":" + PCB_PW;
//           String encoded = base64::encode(credentials);
//           https.addHeader("Content-Type", "application/json");
//           https.addHeader("Authorization", "Basic " + encoded);


//           Convert_To_Json("write" , i+1, "mode"); 


//           int httpCode = https.POST(JsonData);

//           if(httpCode == 200){
//             payload = https.getString();
//             lastState = String(Relays); 

//             digitalWrite(PinOfRelays[i], Relays[i] == '1' ? HIGH : LOW);


//             Serial.println("Button "+String(i+1)+" pressed.");
//           } 
//           else {
//             Serial.println("Failed to update server. Code: "+String(httpCode));
//           }

//           https.end();
//         }

//         lastBottomStates[i] = currentBottomStates[i];


        
//       }



//   delay(500);

// }

void base64Check(){

  https.begin(*client,"https://azarakhsh-test.liara.run/board/mode");
  String credentials = PCB_SN + ":" + PCB_PW;
  String encoded = base64::encode(credentials);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Authorization", "Basic " + encoded);
  int httpCode = https.POST(JsonData);
  if (httpCode == 200)
  {
    payload = https.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("there is a problem : ");
    Serial.println(httpCode);
  }

  https.end();

}



void setup() 
{
  pinMode(0, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  // pinMode( , INPUT);
  // pinMode( , INPUT);
  // pinMode( , INPUT);
  // pinMode( , INPUT);


  display.setBrightness(7);
  for(int i = 0; i < 4; i++) {
    digitalWrite(PinOfRelays[i], LOW);
  }

  Serial.begin(9600); 
  GetInfo();
  doc["wifi_ssid"] = "iPhone";
  doc["wifi_password"] = "Taj527784";
  ChangeJSON();
  WiFiHanddelling();
  delay(500);
}

void loop() 
{ 
  
  stillConnected();

  base64Check();
  CheckSituation();

  // ChangeSituation();

  delay(100);
}

