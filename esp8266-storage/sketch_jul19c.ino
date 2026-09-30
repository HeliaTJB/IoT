#include <LittleFS.h>

File uploadFile;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  File uploadFile = LittleFS.open("/heloo.txt", "w");
  if (!uploadFile) {
    Serial.println("Failed to open file for writing");
    return;
  }

  uploadFile.println("bye");
  uploadFile.close();
  delay(100);

  Serial.println("");
  Serial.println("uploaded file ... ");
}

void loop() {
  delay(1000);
}
