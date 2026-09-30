// #include <EEPROM.h>
#include <RCSwitch.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>            
#include <ArduinoJson.h>    
#include <ESP8266WiFi.h>
#include <TM1637Display.h>
#include <Base64.h>

#define RELAY_COUNT 4
#define MODE_COUNT 6
#define MAX_REMOTES 5

class Time;
class Serverr;
class WiFiManager;
class Remote;
class Relay;
class Clientt;
class Memoryy;
class Menu;


String WIFITEMP;
String PCB_SN;
String PCB_PW;
String Online_Mode;
String Mode;
String Pass;
int Number_Remotes;
String Address_Remote[MAX_REMOTES];
String Mode_Realy[RELAY_COUNT];
int Time_Relay_Start[RELAY_COUNT][2];
int Time_Relay_Stop[RELAY_COUNT][2];
char Relays[RELAY_COUNT] = {'0', '0', '0', '0'};
String remotes[MAX_REMOTES];
const int SLOT_SIZE = 10;
const int MAX_REMOTE = 5;
RCSwitch mySwitch;
bool remoteLearned = false;
unsigned long remoteCodes[4];


base64 Base64;
HTTPClient https;
std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);




class Time {
private:
    int timeOne[2];
    int timeTwo[2];
    int periodOne;
    int periodTwo;

public:
    Time() : timeOne{0,0}, timeTwo{0,0}, periodOne(0), periodTwo(0) {}

    void setTimeOne(int hours, int minutes) {
        timeOne[0] = hours;
        timeOne[1] = minutes;
    }

    void setTimeTwo(int hours, int minutes) {
        timeTwo[0] = hours;
        timeTwo[1] = minutes;
    }

    void setPeriodOne(int minutes) { periodOne = minutes; }
    void setPeriodTwo(int minutes) { periodTwo = minutes; }

    const int* getTimeOne() const { return timeOne; }
    const int* getTimeTwo() const { return timeTwo; }
    int getPeriodOne() const { return periodOne; }
    int getPeriodTwo() const { return periodTwo; }

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


class Serverr {
private:
    const String WiFiURL = "https://azarakhsh-test.liara.run/board/situation";
    const String RelayURL = "https://azarakhsh-test.liara.run/board/mode";
    const String firmwareURL = "https://example.com/firmware.bin";
    
    const char* root_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "-----END CERTIFICATE-----\n";

   
    String jsonData;
    String payload;

public:
    


    String getWiFiURL() {return WiFiURL;}
    String getRelayURL() {return RelayURL;}
    const char* getCA() {return root_ca;}
    String* getJSONData() {return &jsonData;}

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
      jsonData = "{\"Mode\":\"" + mode + "\",\"PCB_SerialNumber\":\"" + PCB_SN + "\",\"PCB_PassWord\":\"" + PCB_PW +  "\",\"RemoteData\":\"" + String(Relays) + "\",\"ExpectedData\":\"" + expectedData + "\"}";
    }

    void connectToServer(String PCB_SN,String PCB_PW, String mode, int n, String mode2){
      https.begin(*client, firmwareURL);
      String credentials = PCB_SN + ":" + PCB_PW;
      String encoded = base64::encode(credentials);
      https.addHeader("Content-Type", "application/json");
      https.addHeader("Authorization", "Basic " + encoded);

      Convert_To_Json(mode, n, mode2);

      int httpCode = https.POST(jsonData);
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
        Serial.println("Checking for updates...");
        t_httpUpdate_return ret = ESPhttpUpdate.update(*client, firmwareURL);

        switch (ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("Update failed (%d): %s\n", 
                            ESPhttpUpdate.getLastError(), 
                            ESPhttpUpdate.getLastErrorString().c_str());
                break;
            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("No updates available");
                break;
            case HTTP_UPDATE_OK:
                Serial.println("Update successful");
                break;
        }
    }

    const String& getPayload() const { return payload; }
};


class Memoryy {
private:
  StaticJsonDocument<1024> doc;
  String ssid;
  String password;

public:

  void updatePCBCredentials(const String& newSN, const String& newPW) {
        doc["pcb"]["serial_number"] = newSN;
        doc["pcb"]["password"] = newPW;
        ChangeJSON();
    }

  void updateWiFiCredentials(const String& newSSID, const String& newPass) {
        doc["wifi"]["ssid"] = newSSID;
        doc["wifi"]["password"] = newPass;
        ChangeJSON();
  }

  String getPCBSerial() {
        return doc["PCB_SN"].as<String>();
  }
    
    String getPCBPassword() {
        return doc["PCB_PW"].as<String>();
    }
    
    String getWiFiSSID() {
        return doc["wifi_ssid"].as<String>();
    }
    
    String getWiFiPassword() {
        return doc["wifi_password"].as<String>();
    }

  void GetInfo() {
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
      Address_Remote[i] = doc["number_remote"][("Address_remote_"+ String(i+1)).c_str()].as<String>();
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

  void ChangeJSON() {
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

  StaticJsonDocument<1024>& returnData() {return doc;}
};

class WiFiManager {
private:
    String ssid;
    String password;
    bool isConfigured;
    Serverr& server;
    Memoryy& memoryy;

public:
    WiFiManager(Serverr& srv, Memoryy& mem) : isConfigured(false), server(srv), memoryy(mem) {}

    void configure(const String& newSSID, const String& newPassword) {
        ssid = newSSID;
        password = newPassword;
        isConfigured = true;
    }

    void connect() {
        if (!isConfigured) return;
        
        WiFi.begin(ssid.c_str(), password.c_str());
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nConnected to WiFi");
    }

    void connectToServerForFirstTime(String PCB_SN, String PCB_PW){
      if(ssid == "iPhone" && password == "Taj527784")
      {
        https.begin(*client, server.getWiFiURL());
        String credentials = PCB_SN + ":" + PCB_PW;
        String encoded = base64::encode(credentials);
        https.addHeader("Authorization", "Basic " + encoded);

        server.Convert_To_Json("read", 0, "wifi");

        int code = https.POST(*server.getJSONData());
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

        WiFi.begin(ssid.c_str(), password.c_str());
        while (WiFi.status() != WL_CONNECTED) {
          Serial.print(".");
          delay(500);
        }
        Serial.println("\nConnected to WiFi from server information");

        memoryy.ChangeJSON();
      }
    }

    bool isConnected() const {
        return WiFi.status() == WL_CONNECTED;
    }

    const String& getSSID() const { return ssid; }
    bool configured() const { return isConfigured; }
};

class Remote {
private:
    RCSwitch rcSwitch;
    int receivePin;
    unsigned long remoteCodes[4];
    bool isLearned;
    static int Count;

public:
    Remote(int pin) : receivePin(pin), isLearned(false) {
        rcSwitch.enableReceive(pin);
        Count++;
    }

    String binaryToHex(const char *bin) {
      if (bin == nullptr)
        return "";
      int len = strlen(bin);
      String hexString = "";
      for (int i = 0; i < len; i += 4) {
        int nibble = 0;
        for (int j = 0; j < 4; j++) {
          if (i + j < len && bin[i + j] == '1') {
            nibble += (1 << (3 - j));
          }
        }
        if (nibble < 10) {
          hexString += char(nibble + '0');
        }
        else {
          hexString += char(nibble - 10 + 'A');
        }
      }
      return hexString;
    }

    int findFirstEmptySlot() {
      for (int i = 0; i < MAX_REMOTE; i++) {
        if (remotes[i].length() == 0)
          return i;
      }
      return -1;
    }

    bool addRemote(const String &code) {
      for (int i = 0; i < MAX_REMOTE; ++i) {
        if (remotes[i] == code) {
          Serial.println("Repeated remote. Try a new one.");
          return false;
        }
      }
      int slot = findFirstEmptySlot();
      if (slot < 0) {
        Serial.println(F("All 5 slots are full!"));
        return false;
      }
      remotes[slot] = code;
      // saveStringToEEPROM(slot * SLOT_SIZE, code);                  //// chnage into little FS
      Serial.print(F("Stored in slot "));
      Serial.print(slot + 1);
      Serial.print(" with code: ");
      Serial.println(code);
      return true;
    }

    bool deleteRemote(int slot) {
      if (slot < 0 || slot >= MAX_REMOTE)
        return false;
      remotes[slot] = "";
      // saveStringToEEPROM(slot * SLOT_SIZE, String(""));             //// chnage into little FS
      Serial.print(F("Deleted slot "));
      Serial.println(slot + 1);
      return true;
    }

    bool clearAllRemotes() {
      for (int i = 0; i < MAX_REMOTE; i++) {
        remotes[i] = "";
        // saveStringToEEPROM(i * SLOT_SIZE, String(""));              //// chnage into little FS
      }
      Serial.println(F("All remote slots cleared."));
      return true;
    }

    String getReceivedCode() {
      if (rcSwitch.available()) {
        const char *binaryValue = rcSwitch.getReceivedBinaryValue();
        String hexValue = binaryToHex(binaryValue);
        rcSwitch.resetAvailable();
        if (hexValue.length() >= 5) {
          return hexValue.substring(0, 5);
        }
      }
      return "";
    }

    int checkButton() {
      if (rcSwitch.available()) {
        const char *binaryValue = rcSwitch.getReceivedBinaryValue();
        String hexValue = binaryToHex(binaryValue);
        String code = hexValue.substring(0, 5);
        rcSwitch.resetAvailable();

        for (int i = 0; i < MAX_REMOTE; i++) {
          if (remotes[i] == code) {
            return i;
          }
        }
      }
      return -1;
    }

    void learnButtons() {
      static int learnStep = 0;
      const char* buttonNames[4] = {"BACK", "OK", "PREV", "NEXT"};

      if (rcSwitch.available()) {
        remoteCodes[learnStep] = rcSwitch.getReceivedValue();
        rcSwitch.resetAvailable();

        Serial.print(buttonNames[learnStep]);
        Serial.print(" button learned: ");
        Serial.println(remoteCodes[learnStep]);

        learnStep++;
        delay(1000);

        if (learnStep >= 4) {
          isLearned = true;
          Serial.println("All remote buttons learned!");
        } else {
          Serial.print("Press ");
          Serial.print(buttonNames[learnStep]);
          Serial.println(" button...");
        }
      }
    }

    bool learned() const { return isLearned; }
    static int getCount() { return Count; }
};
int Remote::Count = 0;

class Relay {
private:
    int pin;
    int mode;
    Time timing;
    bool isConfigured;

public:
    Relay(int p) : pin(p), mode(0), isConfigured(false) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void toggle() {
        digitalWrite(pin, !digitalRead(pin));
    }

    void set(bool state) {
        digitalWrite(pin, state);
    }

    void configure(int newMode, const Time& newTiming) {
        mode = newMode;
        timing = newTiming;
        isConfigured = true;
    }

    int getMode() const { return mode; }
    const Time& getTiming() const { return timing; }
    bool configured() const { return isConfigured; }

    ~Relay() {
      digitalWrite(pin, LOW);
    }
};

class Clientt {
private:
  String pass;
  bool setWiFiID;

public:
  String getPass() {return pass;}
  bool DoesClientSetWiFiID() {return setWiFiID;}

  bool checkEntry(String x) {return (x==pass);}
};



class Menu {
private:
    enum MenuState {
        PASSWORD_ENTRY,
        MAIN_MENU,
        RELAY_SELECTION,
        MODE_SELECTION,
        TIME_SETTING,
        CONFIRMATION
    };

    String main[4] = {"RELY", "EXIT", "SET", "MODE"};

    MenuState currentState;
    int selectedItem;
    int selectedRelay;
    int selectedMode;
    int timeInputState; 
    int currentHours;
    int currentMinutes;
    String enteredPassword;
    Time tempTime;
    TM1637Display display;
    Clientt client;
    Relay* relays[RELAY_COUNT];

    const uint8_t SEG_A = 0b01110111;  // A
    const uint8_t SEG_B = 0b01111100;  // b
    const uint8_t SEG_C = 0b00111001;  // C
    const uint8_t SEG_D = 0b01011110;  // d
    const uint8_t SEG_E = 0b01111001;  // E
    const uint8_t SEG_F = 0b01110001;  // F
    const uint8_t SEG_H = 0b01110110;  // H
    const uint8_t SEG_L = 0b00111000;  // L
    const uint8_t SEG_M = 0b00010101;  // M (custom)
    const uint8_t SEG_O = 0b00111111;  // O
    const uint8_t SEG_P = 0b01110011;  // P
    const uint8_t SEG_R = 0b01010000;  // r
    const uint8_t SEG_S = 0b01101101;  // S
    const uint8_t SEG_T = 0b01111000;  // t
    const uint8_t SEG_U = 0b00111110;  // U
    const uint8_t SEG_Y = 0b01101110;  // Y
    const uint8_t SEG_1 = 0b00001100;
    const uint8_t SEG_2 = 

public:
    Menu(int clkPin, int dioPin) : 
        currentState(PASSWORD_ENTRY),
        selectedItem(0),
        selectedRelay(0),
        selectedMode(0),
        timeInputState(0),
        currentHours(0),
        currentMinutes(0),
        display(clkPin, dioPin) 
    {
        display.setBrightness(0x0a);
    }

    void displayText(const String &text) {
      uint8_t segments[4] = {0x00, 0x00, 0x00, 0x00};
      
      for (int i = 0; i < 4 && i < text.length(); i++) {
            switch(toupper(text[i])) {
                case 'A': segments[i] = SEG_A; break;
                case 'B': segments[i] = SEG_B; break;
                case 'C': segments[i] = SEG_C; break;
                case 'D': segments[i] = SEG_D; break;
                case 'E': segments[i] = SEG_E; break;
                case 'F': segments[i] = SEG_F; break;
                case 'H': segments[i] = SEG_H; break;
                case 'L': segments[i] = SEG_L; break;
                case 'M': segments[i] = SEG_M; break;
                case 'O': segments[i] = SEG_O; break;
                case 'P': segments[i] = SEG_P; break;
                case 'R': segments[i] = SEG_R; break;
                case 'S': segments[i] = SEG_S; break;
                case 'T': segments[i] = SEG_T; break;
                case 'U': segments[i] = SEG_U; break;
                case 'Y': segments[i] = SEG_Y; break;
                case ' ': segments[i] = 0x00; break;
                default: 
                    if (isdigit(text[i])) {
                        segments[i] = display.encodeDigit(text[i] - '0');
                    }
            }
        }
        
      display.showNumberDec(segments, true);
    }

    void handleButtonPress(int button) {
        switch(currentState) {
            case PASSWORD_ENTRY:
                handlePasswordEntry(button);
                break;
            case MAIN_MENU:
                handleMainMenu(button);
                break;
            case RELAY_SELECTION:
                handleRelaySelection(button);
                break;
            case MODE_SELECTION:
                handleModeSelection(button);
                break;
            case TIME_SETTING:
                handleTimeSetting(button);
                break;
            case CONFIRMATION:
                handleConfirmation(button);
                break;
        }
        updateDisplay();
    }

    bool isAuthenticated() const {
        return currentState != PASSWORD_ENTRY;
    }

    int getSelectedRelay() const { return selectedRelay; }
    int getSelectedMode() const { return selectedMode; }
    const Time& getConfiguredTime() const { return tempTime; }

private:
    void handlePasswordEntry(int button) {
        static int currentDigit = 0;
        static int digits[4] = {0, 0, 0, 0}; 
        
        switch(button) {
            case 1: 
                {
                    String enteredPassword = String(digits[0]) + String(digits[1]) + String(digits[2]) + String(digits[3]);
                    if (client.checkEntry(enteredPassword)) {
                        currentState = MAIN_MENU;
                        currentDigit = 0;
                        
                        for (int i = 0; i < 4; i++) { digits[i] = 0;}
                        displayText("OPEN"); 
                        delay(1000);
                    } 
                    else {
                        display.showNumberDec(9999); 
                        delay(1000);
                        currentDigit = 0;
                        for (int i = 0; i < 4; i++) {digits[i] = 0;}
                    }
                }
                break;
                
            case 2:
                currentDigit = (currentDigit + 1) % 4;
                break;
                
            case 4:
                digits[currentDigit] = (digits[currentDigit] + 1) % 10;
                break;
                
            case 8: 
                digits[currentDigit] = (digits[currentDigit] - 1 + 10) % 10;
                break;
        }
        
        int displayDigits[4];
        for (int i = 0; i < 4; i++) {
            if (i == currentDigit && (millis() % 1000 < 500)) {
                displayDigits[i] = 0x00; 
            } 
            else {
                displayDigits[i] = display.encodeDigit(digits[i]);
            }
        }
        int temp = 1000*segment[3] + 100*segment[2] + 10*segment[1] + segment[0];
        display.showNumberDec(temp, true);
    }

    void handleMainMenu(int button) {
        switch(button) {
            case 1: 
                selectedItem = (selectedItem - 1 + 3) % 3;
                break;
            case 2: 
                selectedItem = (selectedItem + 1) % 3;
                break;
            case 4: 
                switch(selectedItem){
                  case 0:
                    currentState = RELAY_SELECTION;
                  break;
                  case 1:
                    // ...
                  break;
                  case 2:
                    // ...
                  break;
                }
                break;
            case 8: 
                currentState = PASSWORD_ENTRY;
                break;
        }
    }

    void handleRelaySelection(int button) {
        switch(button) {
            case 1:
                selectedRelay = (selectedRelay - 1 + RELAY_COUNT) % RELAY_COUNT;
                break;
            case 2: 
                selectedRelay = (selectedRelay + 1) % RELAY_COUNT;
                break;
            case 4: 
                currentState = MODE_SELECTION;
                break;
            case 8: 
                currentState = MAIN_MENU;
                break;
        }
    }

    void handleModeSelection(int button) {
        switch(button) {
            case 1: 
                selectedMode = (selectedMode - 1 + MODE_COUNT) % MODE_COUNT;
                break;
            case 2: 
                selectedMode = (selectedMode + 1) % MODE_COUNT;
                break;
            case 4: 
                if (selectedMode == 1 || selectedMode == 4) { 
                    currentState = TIME_SETTING;
                    timeInputState = 0;
                    currentHours = 0;
                    currentMinutes = 0;
                } 
                else {
                    currentState = CONFIRMATION;
                }
                break;
            case 8: 
                currentState = RELAY_SELECTION;
                break;
        }
    }

    void handleTimeSetting(int button) {
        switch(button) {
            case 1: 
                if (timeInputState == 0) {
                    currentHours = (currentHours - 1 + 24) % 24;
                }
                else {
                    currentMinutes = (currentMinutes - 1 + 60) % 60;
                }
                break;
            case 2:
                if (timeInputState == 0) {
                    currentHours = (currentHours + 1) % 24;
                } 
                else {
                    currentMinutes = (currentMinutes + 1) % 60;
                }
                break;
            case 4: 
                if (timeInputState == 0) {
                    timeInputState = 1;
                } 
                else {
                    tempTime.setTimeOne(currentHours, currentMinutes);
                    currentState = CONFIRMATION;
                }
                break;
            case 8: 
                if (timeInputState == 0) {
                    currentState = MODE_SELECTION;
                } 
                else {
                    timeInputState = 0;
                }
                break;
        }
    }

    void handleConfirmation(int button) {
        if (button == 1) { 
            relays[selectedRelay]->configure(selectedMode, tempTime);
            currentState = MAIN_MENU;
        } 
        else if (button == 0) { 
            currentState = (selectedMode == 1 || selectedMode == 4) ? TIME_SETTING : MODE_SELECTION;
        }
    }

    void updateDisplay() {
        switch(currentState) {
            case PASSWORD_ENTRY:
                break;
            case MAIN_MENU:
                displayText(main[selectedItem]);
                break;
            case RELAY_SELECTION:
                display.showNumberDec(selectedRelay + 1);
                break;
            case MODE_SELECTION:
                break;
            case TIME_SETTING:
                int segments[4];
                segments[0] = display.encodeDigit(currentHours / 10);
                segments[1] = display.encodeDigit(currentHours % 10);
                segments[2] = display.encodeDigit(currentMinutes / 10);
                segments[3] = display.encodeDigit(currentMinutes % 10);
                
                if (timeInputState == 0) { 
                    if (millis() % 1000 < 500) {
                        segments[0] = 0x00;
                        segments[1] = 0x00;
                    }
                } 
                else { 
                    if (millis() % 1000 < 500) {
                        segments[2] = 0x00;
                        segments[3] = 0x00;
                    }
                }
                
                display.showNumberDec(segments, true);
                break;
            case CONFIRMATION:
                displayText("SAVE"); 
                break;
        }
    }
};

class PCB {
private:
    String serialNumber;
    String password;
    WiFiManager wifi;
    Serverr server;
    Remote remote;
    Relay relays[RELAY_COUNT];
    Time time;
    Clientt client;
    Memoryy memoryy;
    Menu menu;
    bool authenticated;
    static String lastPayload;
    TM1637Display display;
    int PinOfRelays[RELAY_COUNT] = {D9, D6, D7, D8};

public:
    PCB() : 
        serialNumber("123456789"),
        password("98765"),
        remote(D5),
        relays{Relay(D9), Relay(D6), Relay(D7), Relay(D8)},
        menu(D3, D4), 
        authenticated(false),
        wifi(server, memoryy),
        display(D3, D4)
    {
        wifi.configure("iPhone", "Taj527784");
        lastPayload = "0000";
    }

    void factoryReset() {
        LittleFS.remove("/info.json");
        ESP.restart();
    }

    void applySettings() {
        int relayNum = menu.getSelectedRelay();
        int mode = menu.getSelectedMode();
        Time relayTime = menu.getConfiguredTime();
        relays[relayNum].configure(mode, relayTime);
    }

    void CheckSituation() {
        if (wifi.configured()) {    
            server.connectToServer(serialNumber, password, "read", 0, "online");
            String payload = server.getPayload();
            
            if (payload.length() == 4) {
                for (int i = 0; i < 4; ++i) {
                    if (payload[i] != lastPayload[i]) {
                        digitalWrite(PinOfRelays[i], payload[i] == '1' ? HIGH : LOW);
                        Serial.print("Relay ");
                        Serial.print(i + 1);
                        Serial.println(payload[i] == '1' ? " ON" : " OFF");
                    }
                }

                int displayValue = payload.toInt(); 
                display.showNumberDec(displayValue);
                
                lastPayload = payload; 
            }
            else {
                Serial.println("Invalid payload received");
                display.showNumberDec(9999); 
            }
        } 
        applySettings();
    }

    void initialize() {
        Serial.begin(9600);

         client.setTrustAnchors(new BearSSL::X509List(server.getCA()));

        time.setTime();
        memoryy.GetInfo();

        // StaticJsonDocument<1024>& doc = memory.returnData();
        // doc["wifi_ssid"] = "iPhone";
        // doc["wifi_password"] = "Taj527784";
        // doc["PCB_SN"] = "5678";
        // doc["PCB_PW"] = "javad1383";

        serialNumber = PCB_SN;  
        password = PCB_PW;
        
        String wifiSSID = memoryy.returnData()["wifi_ssid"].as<String>();
        String wifiPass = memoryy.returnData()["wifi_password"].as<String>();
        
        if (wifiSSID.length() == 0 || wifiPass.length() == 0) {
            wifiSSID = "iPhone";
            wifiPass = "Taj527784";
            memoryy.updateWiFiCredentials(wifiSSID, wifiPass);
        }
        
        wifi.configure(wifiSSID, wifiPass);

        //memory.ChangeJSON();
        wifi.connect();


        wifi.connectToServerForFirstTime(serialNumber, password);

        if (!remote.learned()) {
            remote.learnButtons();
        }
    }

    void run() {
        if (!remote.learned()) {return;}

        int button = remote.checkButton();
        if (button >= 0) {
            menu.handleButtonPress(button);
            CheckSituation();
            
            if (!authenticated && menu.isAuthenticated()) {
                authenticated = true;
            }
        }

        if(wifi.configured()){
          static unsigned long lastCheck = 0;
          if (millis() - lastCheck > 60000 && wifi.isConnected()) {
              server.checkForUpdates();
              lastCheck = millis();
          }
        }
    }
};

String PCB::lastPayload;
PCB pcb;

void setup() {
    pcb.initialize();
}

void loop() {
    pcb.run();
}