#include <WiFi.h>

// WiFi Connection
const char* wifiName = "myWifi1111";
const char* wifiPassw = "Abc123abc";
unsigned int currTime = 0;
unsigned int lastTime = 0;
unsigned int timeInterval = 10000;  // Check every 10 seconds

// Server setup
WiFiServer server(8080);

void serverRequest(char* message) {
  char fullMessage[50] = "";
  strcat(fullMessage, "<html><body><h1>");
  strcat(fullMessage, message);
  strcat(fullMessage, "</h1></body></html>");

  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      Serial.println("Client connected");

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html; charset=UTF-8");
      client.println("Connection: close");
      client.println();

      client.println(fullMessage);
    }
    client.stop();
  }
}

void wifiManipulation(void *pvParameters) {
  // WiFi Initialization
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiName, wifiPassw);
  Serial.println("Connecting to WiFi " + String(wifiName));

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Still connecting....");
    delay(10000);
  }

  Serial.println("Connection successful!");
  IPAddress myIPAddress = WiFi.localIP();
  Serial.println(myIPAddress);

  // Reconnection Check Loop
  while (true) {
    currTime = millis();
    if (currTime - lastTime >= timeInterval) {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost, trying to reconnect");
        WiFi.disconnect();
        WiFi.reconnect();

        // Wait until reconnected
        while (WiFi.status() != WL_CONNECTED) {
          Serial.println("Reconnecting...");
          delay(5000);
        }
        Serial.println("WiFi connection restored!");
      }
      lastTime = currTime;
    }
    delay(500);  
  }
}

// Hardware pins
int pin_magnet = 12;
int pinBuzzer = 27;
int switchState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pin_magnet, INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);

  // Start WiFi and server in separate task
  xTaskCreate(wifiManipulation, "ReconnectingWifiIfDisconnected", 10000, NULL, 1, NULL);
  server.begin();
  Serial.println("Server is up and running");
}

void loop() {
  // Check the magnet switch and respond accordingly
  switchState = digitalRead(pin_magnet);
  if (switchState == LOW) {
    Serial.println("Switch is closed");
    serverRequest("Window closed");
    digitalWrite(pinBuzzer, LOW);
  } else {
    Serial.println("Switch is open");
    serverRequest("Window opened");
    digitalWrite(pinBuzzer, HIGH);
  }
  delay(500);
}
