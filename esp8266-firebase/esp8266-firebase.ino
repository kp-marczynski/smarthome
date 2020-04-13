#include "FirebaseESP8266.h"
#include "ESP8266WiFi.h"
#include "arduino_secrets.h"

String FIREBASE_HOST = SECRET_FIREBASE_HOST;
String FIREBASE_AUTH = SECRET_FIREBASE_AUTH;
String WIFI_SSID = SECRET_WIFI_SSID;
String WIFI_PASSWORD = SECRET_WIFI_PASSWORD;

const int ledPin = LED_BUILTIN;
String path = "/queue";

QueryFilter query;
FirebaseData firebaseData;
FirebaseData firebaseData2;

void setup() {
  Serial.begin(9600);
  Serial.println("Device setup started.");
  setupHardware();
  setupWiFi();
  setupFirebase();
  Serial.println("Device setup finished.");
}

void loop() {
  
}

void setupHardware(){
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void setupWiFi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  clearQueue();
  
  Firebase.setStreamCallback(firebaseData, streamCallback, streamTimeoutCallback);
  if (!Firebase.beginStream(firebaseData, path)) {
      Serial.println("Could not begin stream");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println();
  } else {
    Serial.println("Connected to firebase stream: " + path);
  }
}

void clearQueue() {
  FirebaseJson json;
  json.set("val", "clear");
  Firebase.updateNode(firebaseData, path, json);
}
void blinkLed() {
  Serial.println("Toogle led");
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  digitalWrite(LED_BUILTIN, HIGH);
}

void streamCallback(StreamData data) {
  Serial.println("Stream Data: ");
  Serial.println("Stream path: " + data.streamPath());
  Serial.println("Stream dataPath: " + data.dataPath());
  Serial.println("Stream dataType: " + data.dataType());

  if(data.dataPath() != "/" && data.dataType() == "json") {
//    Firebase.pushJSON(firebaseData2, "/log", data.jsonObject());
    blinkLed();  
    saveLog(data.jsonObject());
    Firebase.deleteNode(firebaseData, path + data.dataPath());
  }
}

void saveLog(FirebaseJson& json) {
  if (Firebase.pushJSON(firebaseData2, "/log", json)) {
    Serial.println(firebaseData2.dataPath());
    Serial.println(firebaseData2.pushName());
    Serial.println(firebaseData2.dataPath() + "/"+ firebaseData.pushName());
  } else {
    Serial.println(firebaseData2.errorReason());
  }
}

void streamTimeoutCallback(bool timeout) {
//  if(timeout){
//    Serial.println("Stream timeout, resume streaming...");
//  }  
}
