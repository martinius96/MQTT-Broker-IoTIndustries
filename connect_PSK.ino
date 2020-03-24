#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>


const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";
const char* mqtt_server = "mqtt.iotindustries.sk";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];
int pocitadlo = 0;
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.loop();
  if (millis() - lastMsg > 10000) {
    char pocitadlo_pole[8];
    pocitadlo  = pocitadlo + 1;
    dtostrf(pocitadlo, 1, 2, pocitadlo_pole);
    client.publish("esp32/pocitadlo", pocitadlo_pole);
    client.subscribe("esp32/pocitadlo");
    lastMsg = millis();
  }
}
