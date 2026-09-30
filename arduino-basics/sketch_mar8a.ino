#include <TM1637Display.h>

TM1637Display display(12,13);
void setup() {
  // put your setup code here, to run once:
  display.setBrightness(7);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = 444;
  display.showNumberDec(x);
  delay(1000);
  display.clear();
  delay(1000);
  uint8_t y[4] = {0x77,0x7c,0x39,0x5e};

  display.setSegments(y);
  delay(1000);
  display.clear();
}
