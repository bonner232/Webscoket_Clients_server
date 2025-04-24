#include <WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "******";
const char* password = "*********";
const char* websocket_server = "192.168.1.68"; // Replace with your server IP
const uint16_t websocket_port = 6789;          // Your server's port

WebSocketsClient webSocket;

void onWebSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket disconnected.");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket connected.");
      webSocket.sendTXT("Hello from ESP32");
      break;
    case WStype_TEXT:
      Serial.print("Message from server: ");
      Serial.println((char*)payload);
      break;
    case WStype_BIN:
      Serial.println("Binary message received (ignored).");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi. IP: " + WiFi.localIP().toString());

  // Setup WebSocket
  webSocket.begin(websocket_server, websocket_port, "/");
  webSocket.onEvent(onWebSocketEvent);
  webSocket.setReconnectInterval(5000); // Retry every 5s if disconnected
}

void loop() {
  webSocket.loop();

  // Example: send something every 5 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 5000) {
    lastSend = millis();
    webSocket.sendTXT("ESP32: ping");
  }
}
