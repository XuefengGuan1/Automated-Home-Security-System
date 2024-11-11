#include <WiFi.h>

// WiFi Connection
const char* wifiName = "myWifi1111";
const char* wifiPassw = "Abc123abc";
unsigned int currTime = 0;
unsigned int lastTime = 0;
unsigned int timeInterval = 10000;  // Check every 10 seconds

// Server setup
WiFiServer server(8080);

void serverRequest(const char* message) {
  char fullMessage[100] = "";
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

// Hardware pins for each sensor
// Window 1
int pin_magnet1 = 12; 
// Window 2
int pin_magnet2 = 14; 
// Motion Sensor
int pin_motion = 16;  
int pinBuzzer = 27;

// Task to monitor Window 1
void monitorWindow1(void *pvParameters) {
  while (true) {
    int switchState = digitalRead(pin_magnet1);
    if (switchState == LOW) {
      Serial.println("Window 1 is closed");
      serverRequest("Window 1 closed");
      digitalWrite(pinBuzzer, LOW);
    } else {
      Serial.println("Window 1 is open");
      serverRequest("Window 1 opened");
      digitalWrite(pinBuzzer, HIGH);
    }
    delay(500);
  }
}

// Task to monitor Window 2
void monitorWindow2(void *pvParameters) {
  while (true) {
    int switchState = digitalRead(pin_magnet2);
    if (switchState == LOW) {
      Serial.println("Window 2 is closed");
      serverRequest("Window 2 closed");
      digitalWrite(pinBuzzer, LOW);
    } else {
      Serial.println("Window 2 is open");
      serverRequest("Window 2 opened");
      digitalWrite(pinBuzzer, HIGH);
    }
    delay(500);
  }
}

// Task to monitor Motion Sensor
void monitorMotionSensor(void *pvParameters) {
  while (true) {
    int motionDetected = digitalRead(pin_motion);
    if (motionDetected == LOW) {
      Serial.println("No motion detected");
      serverRequest("No motion");
      digitalWrite(pinBuzzer, LOW);
    } else {
      Serial.println("Motion detected");
      serverRequest("Motion detected");
      digitalWrite(pinBuzzer, HIGH);
    }
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pin_magnet1, INPUT_PULLUP);
  pinMode(pin_magnet2, INPUT_PULLUP);
  pinMode(pin_motion, INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);

  // Start WiFi and server in separate task
  xTaskCreate(wifiManipulation, "ReconnectingWifiIfDisconnected", 10000, NULL, 1, NULL);
  server.begin();
  Serial.println("Server is up and running");

  // Start tasks for each sensor
  xTaskCreate(monitorWindow1, "MonitorWindow1", 10000, NULL, 1, NULL);
  xTaskCreate(monitorWindow2, "MonitorWindow2", 10000, NULL, 1, NULL);
  xTaskCreate(monitorMotionSensor, "MonitorMotionSensor", 10000, NULL, 1, NULL);
}

void loop() {
}
