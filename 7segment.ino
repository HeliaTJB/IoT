#include <TM1637Display.h>

TM1637Display temp(13,12);

void setup() {
  // put your setup code here, to run once:
  temp.setBrightness(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  temp.showNumberDec(1707);
  delay(2000);
  temp.clear();
  delay(2000);
}
