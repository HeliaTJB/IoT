#include <EEPROM.h>


String readStringFromEEPROM(int addr) {
int len = EEPROM.read(addr);
char data[len + 1]; /
for (int i = 0; i < len; i++) {
data[i] = EEPROM.read(addr + 1 + i); 
}
data[len] = '\0'; 
return String(data); 
}

void saveStringToEEPROM(int addr, const String& str) {
int len = str.length();
EEPROM.write(addr, len);
for (int i = 0; i < len; i++) {
EEPROM.write(addr + 1 + i, str[i]); 
EEPROM.commit(); 
}

