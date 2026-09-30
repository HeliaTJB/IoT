#include <EEPROM.h>
#include <RCSwitch.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>            
#include <ArduinoJson.h>    
#include <ESP8266WiFi.h>
#include <TM1637Display.h>
#include <Base64.h>


class Time {
private:
  int timeOne[2];
  int timeTwo[2];
  int periodOne;
  int periodTwo;

public:
  Time() {
    timeOne[0] =0;
    timeOne[1] =0;
    timeTwo[0] =0;
    timeTwo[1] =0;
    periodOne =0;
    periodTwo =0;
  }

  void setTimeOne(int hours, int minutes) {
    timeOne[0] =hours;
    timeOne[1] =minutes;
  }

  void setTimeTwo(int hours, int minutes) {
    timeTwo[0] = hours;
    timeTwo[1] = minutes;
  }

  void setPeriodOne(int minutes) {
    periodOne = minutes;
  }

  void setPeriodTwo(int minutes) {
    periodTwo = minutes;
  }

  int* getTimeOne[2] () { return timeOne; }
  int* getTimeTwo[2] () { return timeTwo; }
  int getPeriodOne() { return periodOne; }
  int getPeriodTwo() { return periodTwo; }

  void setTime() {
    configTime(3.5 * 3600, 0, "pool.ntp.org", "time.google.com"); 
    Serial.print("Waiting for time to be set...");
    while (time(nullptr) < 8 * 3600 * 2) { 
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nTime is set.");
  }

};

class Server{
private:
  String WiFiURL = "https://azarakhsh-test.liara.run/board/situation";
  String RelayURL = "https://azarakhsh-test.liara.run/board/mode";
  
  const char* root_ca = \
  "-----BEGIN CERTIFICATE-----\n" \
  "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\n" \
  "-----END CERTIFICATE-----\n";

public:
  String getWiFiURL() {return WiFiURL;}
  String getReayURL() {return RelayURL;}
  const char* getCA() {return root_ca;}

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

  void connectToServer(String URL, String mode, int n, String mode2){

    https.begin(*client, URL);
    String credentials = PCB_SN + ":" + PCB_PW;
    String encoded = base64::encode(credentials);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", "Basic " + encoded);


    Convert_To_Json(mode, n,  mode2);


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

  void checkForUpdates() {
    Serial.println("🔄 Checking for firmware update...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmware_url);

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("❌ Update failed. Error (%d): %s\n", 
                      ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("ℹ️ No updates available.");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("✅ Update successful.");
        break;
    }
  }

};

class WiFi {
private:
  String ssid;
  String pass;
  bool Mode;

public:
  WiFi() : Mode(false) {}
  
  void setup(String s, String p) {
    ssid = s;
    pass = p;
    Mode = true;
  }

  String getSSID() { return ssid; }
  String getPass() { return pass; }
  bool isModeOn() { return Mode; }

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


    client->setTrustAnchors(&server.getCA());


    if(ssid == "iPhone" && password == "Taj527784")
    {
    https.begin(*client,server.getWiFiURL());
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

    memory.ChangeJSON();
    
    }
  }

};

class Remote {
private:
  static int number;
  unsigned long remoteCodes[4];
  bool remoteLearned;
  int receivePin;

public:
  Remote(int pin, int n) : receivePin(pin), remoteLearned(false), number(n) {
    mySwitch.enableReceive(receivePin);
  }

  bool isLearned() { return remoteLearned; }

  void learnButtons() {
    static int learnStep = 0;
    const char* buttonNames[4] = {"BACK", "OK", "PREV", "NEXT"};

    if (mySwitch.available()) {
      remoteCodes[learnStep] = mySwitch.getReceivedValue();
      mySwitch.resetAvailable();

      Serial.print(buttonNames[learnStep]);
      Serial.print(" button learned: ");
      Serial.println(remoteCodes[learnStep]);

      learnStep++;
      delay(1000);

      if (learnStep >= 4) {
        remoteLearned = true;
        Serial.println("All remote buttons learned!");
      } else {
        Serial.print("Press ");
        Serial.print(buttonNames[learnStep]);
        Serial.println(" button...");
      }
    }
  }

  int checkButton() {
    if (mySwitch.available()) {
      unsigned long code = mySwitch.getReceivedValue();
      mySwitch.resetAvailable();

      for (int i = 0; i < 4; i++) {
        if (code == remoteCodes[i]) {
          return i;
        }
      }


    }
    return -1;
  }


};
static int Remote::number;

class Relay {
private:
  int pin;
  int mode;
  Time timing;
  bool configured;

public:
  Relay(int p) : pin(p), mode(0), configured(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  void toggle() {
    digitalWrite(pin, !digitalRead(pin));
  }

  void activate(bool state) {
    digitalWrite(pin, state);
  }

  void setMode(int newMode) {
    mode = newMode;
    configured = true;
  }

  void setTime(Time t) {
    timing = t;
  }

  int getMode() { return mode; }
  Time getTime() { return timing; }
  bool isConfigured() { return configured; }

  const char* getModeName() {
    switch(mode) {
      case 0: return "Manual Toggle";
      case 1: return "Duration";
      case 2: return "Hold Remote";
      case 3: return "Remote Cycle";
      case 4: return "Schedule Cycle";
      case 5: return "Start-Stop";
      default: return "Unknown";
    }
  }


};

class Menu {
private:
  int menuState;
  int timeInputState;
  int selectedRelay;
  int selectedMode;
  int inputIndex;
  int currentHours;
  int currentMinutes;
  Time tempTime;

public:
  Menu() : menuState(0), timeInputState(0), selectedRelay(0), selectedMode(0), inputIndex(0), currentHours(0), currentMinutes(0) {}

  void handleEvent(int button) {
    switch(button) {

      case 0: handleBack(); break;
      case 1: handleOK(); break;
      case 2: handlePrev(); break;
      case 3: handleNext(); break;
    }
    display();
  }

  int getSelectedRelay() { return selectedRelay; }
  int getSelectedMode() { return selectedMode; }
  Time getTempTime() { return tempTime; }

  void handleNext() { //// mode for main menu 
    if (menuState == 0) {
      selectedRelay = (selectedRelay + 1) % RELAY_COUNT;
    } 
    else if (menuState == 1) {
      selectedMode = (selectedMode + 1) % MODE_COUNT;
    }
    else if (menuState == 2) {
      if (timeInputState == 0) {
        currentHours = (currentHours + 1) % 24;
      } else {
        currentMinutes = (currentMinutes + 1) % 60;
      }
      updateTempTime();
    }
  }

  void handlePrev() {
    if (menuState == 0) {
      selectedRelay = (selectedRelay - 1 + RELAY_COUNT) % RELAY_COUNT;
    }
    else if (menuState == 1) {
      selectedMode = (selectedMode - 1 + MODE_COUNT) % MODE_COUNT;
    }
    else if (menuState == 2) {
      if (timeInputState == 0) {
        currentHours = (currentHours - 1 + 24) % 24;
      } else {
        currentMinutes = (currentMinutes - 1 + 60) % 60;
      }
      updateTempTime();
    }
  }

  // void updateTempTime() {
  //   if (inputIndex < 2) {
  //     tempTime.setTimeOne(currentHours, currentMinutes);
  //   } 
  //   else {
  //     tempTime.setPeriodOne((currentHours * 60) + currentMinutes);
  //   }
  // }

  void handleOK() {
    if (menuState == 0) {
      menuState = 1;
    }
    else if (menuState == 1) {
      menuState = 2;
      inputIndex = 0;
      tempTime = Time();
      timeInputState = 0;
      currentHours = 0;
      currentMinutes = 0;
    }
    else if (menuState == 2) {
      if (timeInputState == 0) {
        timeInputState = 1;
      } else {
        timeInputState = 0;
        inputIndex++;
        if (inputIndex >= 4) {
          menuState = 3;
        }
      }
    }
    else if (menuState == 3) {
      menuState = 0;
    }
  }

  void handleBack() {
    if (menuState == 1) {
      menuState = 0;
    }
    else if (menuState == 2) {
      if (timeInputState == 1) {
        timeInputState = 0;
      } else {
        menuState = 1;
      }
    }
    else if (menuState == 3) {
      menuState = 2;
      timeInputState = 1;
      inputIndex = 3;
    }
  }


  void display() {
    Serial.println();
    if (menuState == 0) {
      Serial.print("Select Relay: ");
      Serial.println(selectedRelay + 1);
    }
    else if (menuState == 1) {
      Serial.print("Mode for Relay ");
      Serial.print(selectedRelay + 1);
      Serial.print(": ");
      Serial.println(getModeName(selectedMode));
    }
    else if (menuState == 2) {
      Serial.print("Time[");
      Serial.print(inputIndex);
      Serial.print("] ");
      Serial.print(timeInputState == 0 ? ">" : " ");
      if (currentHours < 10) Serial.print("0");
      Serial.print(currentHours);
      Serial.print(":");
      Serial.print(timeInputState == 1 ? ">" : " ");
      if (currentMinutes < 10) Serial.print("0");
      Serial.println(currentMinutes);
    }
    else if (menuState == 3) {
      Serial.println("Confirm Settings:");
      Serial.print("Relay "); Serial.print(selectedRelay + 1);
      Serial.print(" Mode: "); Serial.println(getModeName(selectedMode));
    }
  }


  const char* getModeName(int mode) {
    switch(mode) {
      case 0: return "Manual Toggle";
      case 1: return "Duration";
      case 2: return "Hold Remote";
      case 3: return "Remote Cycle";
      case 4: return "Schedule Cycle";
      case 5: return "Start-Stop";
      default: return "Unknown";
    }
  }


};

class Client{
private:
  String pass;
  bool setWiFiID;

public:
  void getPass() {return pass;}
  bool DoesClientSetWiFiID() {return setWiFiID;}

  void checkEntry() {... }
};

class Memory{
private:
  StaticJsonDocument<1024> doc;

public:

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

};

class PCB {
private:
  String SN;
  String PW;
  WiFi wifi;
  Remote remote[5];
  Menu menu;
  Relay relays[4];
  Client tempClient;
  Time time;

public:
  PCB() : SN("123456789"), PW("98765"), remote(D5) {
    wifi.setup("iPhone", "Taj527784");
    relays[0] = Relay(D9);
    relays[1] = Relay(D6);
    relays[2] = Relay(D7);
    relays[3] = Relay(D8);
  }


  void initialize() {
    Serial.begin(9600);

    RCSwitch mySwitch;
    BearSSL::X509List cert(root_ca);
    base64 Base64;

    HTTPClient https;
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    time.setTime();

    Serial.println("--- Relay Manager ---");
    if (!remote.isLearned()) {
      Serial.println("Press each remote button once (BACK, OK, PREV, NEXT)");
    }
  }

  void run() {
    if (!remote.isLearned()) {
      remote.learnButtons();
      return;
    }

    int button = remote.checkButton();
    if (button >= 0) {
      menu.handleEvent(button);
      delay(200);
      
      if (menu.getSelectedMode() == 0) {
        relays[menu.getSelectedRelay()].toggle();
      }
    }

    CheckSituation();

  }


  void applySettings() {
    int relayNum = menu.getSelectedRelay();
    relays[relayNum].setMode(menu.getSelectedMode());
    relays[relayNum].setTime(menu.getTempTime());
    Serial.print("Settings applied to relay ");
    Serial.println(relayNum + 1);
  }

  void CheckSituation(){
    // for(int i = 0; i < 4; ++i) {
    //   if(payload[i]!=lastState[i]){
    //     if(payload[i]=='0'){
    //       digitalWrite(PinOfRelays[i], LOW);
    //     }
    //     else {
    //       digitalWrite(PinOfRelays[i], HIGH);
    //     }
    //     lastState[i] = payload[i];
    //     }
    
    // }

    // int temp = payload.toInt();
    // temp = temp + 1000;
    // display.showNumberDec(temp);
  }

};


PCB pcb;
Server server;


void setup() {
  pcb.initialize();
}

void loop() {
  pcb.run();
}


