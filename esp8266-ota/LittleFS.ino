#include <ESP8266WiFi.h>      // For ESP8266
// #include <WiFi.h>          // Uncomment for ESP32
#include <ESP8266WebServer.h> // For ESP8266
// #include <WebServer.h>     // Uncomment for ESP32
#include <LittleFS.h>

// WiFi Credentials
const char* ssid = "iPhone";
const char* password = "Taj527784";

// Create a web server
ESP8266WebServer server(80);  // For ESP8266
// WebServer server(80);       // For ESP32

// HTML Form for File Upload
const char* htmlForm = 
"<html>"
"<head><title>ESP File Upload</title></head>"
"<body>"
"<h1>Upload a File to ESP</h1>"
"<form method='POST' action='/upload' enctype='multipart/form-data'>"
"<input type='file' name='file'><br><br>"
"<input type='submit' value='Upload'>"
"</form>"
"</body>"
"</html>";

void setup() {
  Serial.begin(9600);
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully!");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlForm);
  });

  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/plain", "File uploaded successfully!");
  }, handleFileUpload);

  server.begin();
  Serial.println("HTTP server started!");
}

void loop() {
  server.handleClient();
}

// Handle file upload
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.print("Uploading: ");
    Serial.println(filename);
    
    // Open the file for writing (delete if it exists)
    File file = LittleFS.open(filename, "w");
    if (!file) {
      Serial.println("Failed to open file for writing!");
      return;
    }
    file.close();
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    File file = LittleFS.open(upload.filename, "a");
    if (file) {
      file.write(upload.buf, upload.currentSize);
      file.close();
    }
  } 
  else if (upload.status == UPLOAD_FILE_END) {
    Serial.print("Uploaded: ");
    Serial.print(upload.filename);
    Serial.print(" (");
    Serial.print(upload.totalSize);
    Serial.println(" bytes)");
  }
}