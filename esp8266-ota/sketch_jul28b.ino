#include <EEPROM.h>
#include <RCSwitch.h>


class Time{
private:
  int Mode;
  int timeOne[2];
  int timeTwo[2];
  int periodOne;
  int periodTwo;

public:
  Time() {
    timeOne[0] =0;
    timeTwo[0] =0;
    timeOne[1] =0;
    timeTwo[1] =0;
    periodOne =0;
    periodTwo =0;
  }

  void setTimeOne(int n, int m){timeOne[0]=n; timeOne[1]=m;}
  void setTimeTwo(int n, int m){timeTwo[0]=n; timeTwo[1]=m;}
  void set periodOne(int n) {periodOne =n;}
  void set periodTwo(int n) {periodTwo =n;}
  int getMode() {return Mode;}
  int getperiodOne() {return periodOne;}
  int getperiodTwo() {return periodTwo;}
  int getTimeOne[2] () {return timeOne;}
  int getTimeTwo[2] () {return timeTwo;}

};

class WiFi{
private:
  String ssid;
  String pass;
  bool Mode;

public:
  WiFi() : Mode(true) {}
  WiFi(String s, String p) : ssid(s), pass(p) {}

  String getssid() {return ssid;}
  String getpass() {return pass;}
  bool getMode() {return Mode;}

};


class remote{
private:
  RCSwitch mySwitch = RCSwitch();
  static int number;
  String address;
  unsigned long remoteCodes[4]; // BACK, OK, PREV, NEXT
  bool remoteLearned = false;

public:
  void learnRemoteButtons() {
  static int learnStep = 0;
  const char* buttonNames[4] = {"BACK", "OK", "PREV", "NEXT"};

  if (mySwitch.available()) {
    unsigned long code = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();

    remoteCodes[learnStep] = code;
    Serial.print(buttonNames[learnStep]);
    Serial.print(" button learned: ");
    Serial.println(code);

    learnStep++;
    delay(1000);

    if (learnStep >= 4) {
      remoteLearned = true;
      Serial.println("All remote buttons learned!");
      displayMenu();
    } else {
      Serial.print("Press ");
      Serial.print(buttonNames[learnStep]);
      Serial.println(" button...");
    }
  }
}

};
static remote::number;


class Relay{
private:
  int Mode;
  time timehaddeling;
  int pin;
  bool configured; 

public:
  void LowPinMode(int relayPin) {
    pin = relayPin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  void toggle() {
    digitalWrite(pin, !digitalRead(pin));
  }
  void activate(bool state) {
    digitalWrite(pin, state ? HIGH : LOW);
  }

  void setMode(int newMode) { mode = newMode;}
  void setTime(Time temp) {timehaddeling = temp;}
  void getMopde() {return Mode;}

  const char* getModeName(RelayMode mode) {
  switch (mode) {
    case MODE_TOGGLE: return "Manual Toggle";
    case MODE_DURATION: return "Duration";
    case MODE_HOLD_REMOTE: return "Hold Remote";
    case MODE_REMOTE_CYCLE: return "Remote Cycle";
    case MODE_SCHEDULE_CYCLE: return "Schedule Cycle";
    case MODE_START_STOP: return "Start-Stop";
    default: return "Unknown";
  }
}
  

};

class Menu{
private:
public:
  
void handleNext() {
  switch (menuState) {
    case MENU_SELECT_RELAY:
      selectedRelay = (selectedRelay + 1) % RELAY_COUNT;
      break;
    case MENU_SELECT_MODE:
      selectedMode = (selectedMode + 1) % MODE_COUNT;
      break;
    case MENU_INPUT_TIME:
      if (timeInputSubState == INPUT_HOURS) {
        currentHours = (currentHours + 1) % 24; // Hours range: 0-23
      } else {
        currentMinutes = (currentMinutes + 1) % 60; // Minutes range: 0-59
      }
      tempTimeValues[inputIndex] = currentHours * 60 + currentMinutes;
      break;
    default: break;
  }
  displayMenu();
}


  void handlePrev() {
  switch (menuState) {
    case MENU_SELECT_RELAY:
      selectedRelay = (selectedRelay - 1 + RELAY_COUNT) % RELAY_COUNT;
      break;
    case MENU_SELECT_MODE:
      selectedMode = (selectedMode - 1 + MODE_COUNT) % MODE_COUNT;
      break;
    case MENU_INPUT_TIME:
      if (timeInputSubState == INPUT_HOURS) {
        currentHours = (currentHours - 1 + 24) % 24;
      } else {
        currentMinutes = (currentMinutes - 1 + 60) % 60;
      }
      tempTimeValues[inputIndex] = currentHours * 60 + currentMinutes;
      break;
    default: break;
  }
  displayMenu();
}

void handleOK() 
{
  switch (menuState) 
  {
    case MENU_SELECT_RELAY:
      menuState = MENU_SELECT_MODE;
      break;
    case MENU_SELECT_MODE:
      menuState = MENU_INPUT_TIME;
      inputIndex = 0;
      memset(tempTimeValues, 0, sizeof(tempTimeValues));
      timeInputSubState = INPUT_HOURS;
      currentHours = 0;
      currentMinutes = 0;
      break;
    case MENU_INPUT_TIME:
      if (timeInputSubState == INPUT_HOURS) {
        timeInputSubState = INPUT_MINUTES;
      } else {
        timeInputSubState = INPUT_HOURS;
        inputIndex++;
        if (inputIndex >= TIME_VALUES_MAX) {
          menuState = MENU_CONFIRM;
        }
      }
      break;
    case MENU_CONFIRM:
      applySettings();
      menuState = MENU_SELECT_RELAY;
      break;
  }
  displayMenu();
}

void handleBack() {
  if (menuState == MENU_SELECT_MODE) {
    menuState = MENU_SELECT_RELAY;
  } else if (menuState == MENU_INPUT_TIME) {
    if (timeInputSubState == INPUT_MINUTES) {
      timeInputSubState = INPUT_HOURS;
    } else {
      menuState = MENU_SELECT_MODE;
    }
  } else if (menuState == MENU_CONFIRM) {
    menuState = MENU_INPUT_TIME;
    timeInputSubState = INPUT_MINUTES;
    inputIndex = TIME_VALUES_MAX - 1;
  }
  displayMenu();
}

void displayMenu() {
  Serial.println();
  switch (menuState) {
    case MENU_SELECT_RELAY:
      Serial.print("Select Relay: ");
      Serial.println(selectedRelay + 1);
      break;
    case MENU_SELECT_MODE:
      Serial.print("Mode for Relay ");
      Serial.print(selectedRelay + 1);
      Serial.print(": ");
      Serial.println(getModeName((RelayMode)selectedMode));
      break;
    case MENU_INPUT_TIME:
      Serial.print("Time[");
      Serial.print(inputIndex);
      Serial.print("] ");
      Serial.print(timeInputSubState == INPUT_HOURS ? ">" : " ");
      Serial.print(currentHours < 10 ? "0" : "");
      Serial.print(currentHours);
      Serial.print(":");
      Serial.print(timeInputSubState == INPUT_MINUTES ? ">" : " ");
      Serial.print(currentMinutes < 10 ? "0" : "");
      Serial.println(currentMinutes);
      break;
    case MENU_CONFIRM:
      Serial.println("Confirm Settings:");
      Serial.print("Relay "); Serial.print(selectedRelay + 1);
      Serial.print(" Mode: "); Serial.println(getModeName((RelayMode)selectedMode));
      for (int i = 0; i < TIME_VALUES_MAX; i++) {
        Serial.print("T");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(tempTimeValues[i] / 60);
        Serial.print(":");
        Serial.print(tempTimeValues[i] % 60 < 10 ? "0" : "");
        Serial.println(tempTimeValues[i] % 60);
      }
      break;
  }
}


};


class PCB{
private:
  String SN;
  String PW;
  String pass;
  WiFi wifihanddeling;
  Menu menu;
  remote remotes[5];
  relay relays[4];


public:
  PCB() {
    SN = "123456789";
    PW = "98765";
    wifi("iPhone", "Taj527784");
  }

  void initialize() {
    Serial.begin(9600);

    for (int i = 0; i < RELAY_COUNT; i++) {
      pinMode(RELAY_PINS[i], OUTPUT);
      digitalWrite(RELAY_PINS[i], LOW);
    }

    mySwitch.enableReceive(D5);

    Serial.println("--- Relay Manager ---");
    Serial.println("Please press each remote button once to learn them (BACK, OK, PREV, NEXT)");

  }

  void run() {
    if (!remoteLearned) {
    learnRemoteButtons();
    return;
  }

  if (mySwitch.available()) {
    unsigned long code = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();

    if (code == remoteCodes[0]) {
      Serial.println("BACK button");
      handleBack();
    } else if (code == remoteCodes[1]) {
      Serial.println("OK button");
      handleOK();
    } else if (code == remoteCodes[2]) {
      Serial.println("PREV button");
      handlePrev();
    } else if (code == remoteCodes[3]) {
      Serial.println("NEXT button");
      handleNext();
    } else {
      Serial.print("Unknown remote code: ");
      Serial.println(code);
    }

    delay(200);
    }
  }

  void applySettings() {
  RelayConfig &cfg = relayConfigs[selectedRelay];
  cfg.mode = (RelayMode)selectedMode;
  memcpy(cfg.timeValues, tempTimeValues, sizeof(tempTimeValues));
  cfg.configured = true;
  Serial.println("Settings applied.");
  }

};



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
