#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char* ssid = "your_wifi_network_name";
const char* password = "your_wifi_password";

const char* HtmlMime = "text/html";
const char* PngMime = "image/png";
const char* CssMime = "text/css";
const char* JsMime = "text/javascript";

const char* getFileMimeType(String extension) {
  if (extension == "html") return HtmlMime;
  if (extension == "css") return CssMime;
  if (extension == "js") return JsMime;
  if (extension == "png") return PngMime;
  return "";
}

ESP8266WebServer server(80);

void setupEndpointsForPath(String path) {
  // Directory instance for the specified path
  Dir element = LittleFS.openDir(path);

  // Go through each element in the directory
  // This call sets the element instance to the next element in the path and returns true
  // The element can be a sub-directory or a file
  // If there are no more elements in the path the call returns false
  while (element.next())
  {
    // If the element is a directory, do a recursive call
    if (element.isDirectory()) {
      setupEndpointsForPath(element.fileName());
    } else {
      // Element is a file, so let's open it
      File file = element.openFile("r");

      // Get filename, we'll need it later (e.g. index.html)  
      String filename = element.fileName();

      // Get the full name (e.g. css/main.css)
      String fullName = file.fullName();

      // Get the file's extension
      String extension = filename.substring(filename.lastIndexOf(".") + 1);

      // Get the mime type for the file
      const char* mimeType = getFileMimeType(extension);
      
      // Get the content size, create an array to act as buffer and read all data from the file into the buffer
      size_t contentSize = file.size();
      uint8_t* content = new uint8_t[contentSize];
      file.read(content, contentSize);

      // Close the file
      file.close();

      Serial.println(fullName);
      Serial.println(extension);
      Serial.println(filename);
      Serial.println(mimeType);

      // Setup server callback for the full path of the file
      server.on("/" + fullName, [mimeType, content, contentSize]() {
        // Serve the file when its URL is called
        server.send(200, mimeType, content, contentSize);
      });

      // Serve index.html on default path
      if (fullName == "index.html") {
        server.on("/", [mimeType, content, contentSize]() {
          server.send(200, mimeType, content, contentSize);
        });
      }
    }    
  }  
}

void setupEndpoints() {
  LittleFS.begin();
  setupEndpointsForPath("/");

  server.onNotFound([]() {
    server.send(404, "text/plain", "Could not find the requested content");
  });
  server.begin();
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  setupEndpoints();

  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}