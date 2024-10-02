#include <WiFi.h>

//Wifi Connection
const char* wifiName = "myWifi1111";
const char* wifiPassw = "Abc123abc";
void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiName, wifiPassw);
  Serial.println("Connecting to wifi" + String(wifiName));

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Still connecting....");
    delay(10000);
  }

  Serial.println("Connection successful!");
}

//Wifi Reconnection
unsigned int currTime = 0;
unsigned int lastTime = 0;
unsigned int timeInterval = 10000;  //Check every 10 seconds
void wifiReconnection() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi connection lost, trying to reconnect");
    WiFi.disconnect();
    WiFi.reconnect();
  }
  Serial.println("Wifi connection back");
}

//Server setup
WiFiServer server(8080);
void initServer(){
    server.begin();
    Serial.println("Server is up and running");
}

int pin_magnet = 12;
int pinBuzzer = 27;
int switchState = 0;
void setup() {
  Serial.begin(115200);
  initWifi();
  initServer();
  pinMode(pin_magnet, INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
  currTime = millis();
  if (currTime - lastTime >= timeInterval) {
    if (WiFi.status() != WL_CONNECTED) {
      wifiReconnection();
    }
    lastTime = currTime;
  }

  WiFiClient client = server.available();
  if(client){
    if(client.connected()){
      Serial.println("client connected");
    }
    client.stop();
  }

  switchState = digitalRead(pin_magnet);
  if (switchState == LOW) {
    Serial.println("switch is closed");
    digitalWrite(pinBuzzer, LOW);
  } else {
    Serial.println("switch is open");
    digitalWrite(pinBuzzer, HIGH);
  }
  delay(500);
}
