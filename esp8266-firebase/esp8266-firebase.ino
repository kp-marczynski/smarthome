#include "FirebaseESP8266.h"
#include "ESP8266WiFi.h"
#include "arduino_secrets.h"

String FIREBASE_HOST = SECRET_FIREBASE_HOST;
String FIREBASE_AUTH = SECRET_FIREBASE_AUTH;
String WIFI_SSID = SECRET_WIFI_SSID;
String WIFI_PASSWORD = SECRET_WIFI_PASSWORD;

String queueEndpoint = "/queue";
String historyEndpoint = "/history";

const int ledPin = LED_BUILTIN;

QueryFilter query;
FirebaseData firebaseQueue;
FirebaseData firebaseHistory;

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
  
  Firebase.setStreamCallback(firebaseQueue, streamCallback, streamTimeoutCallback);
  if (!Firebase.beginStream(firebaseQueue, queueEndpoint)) {
      Serial.println("Could not begin stream");
      Serial.println("REASON: " + firebaseQueue.errorReason());
      Serial.println();
  } else {
    Serial.println("Connected to firebase stream: " + queueEndpoint);
  }
}

void clearQueue() {
  FirebaseJson json;
  json.set("val", "clear");
  Firebase.updateNode(firebaseQueue, queueEndpoint, json);
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
    blinkLed();
    saveLog(data.jsonObject());
    Firebase.deleteNode(firebaseQueue, queueEndpoint + data.dataPath());
  }
}

void saveLog(FirebaseJson& json) {
  if (Firebase.pushJSON(firebaseHistory, historyEndpoint, json)) {
    Serial.println(firebaseHistory.dataPath());
    Serial.println(firebaseHistory.pushName());
    Serial.println(firebaseHistory.dataPath() + "/"+ firebaseQueue.pushName());
  } else {
    Serial.println(firebaseHistory.errorReason());
  }
}

void streamTimeoutCallback(bool timeout) {
//  if(timeout){
//    Serial.println("Stream timeout, resume streaming...");
//  }  
}
