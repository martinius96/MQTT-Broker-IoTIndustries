#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include <SPI.h>
#include <Ethernet.h>
//#include <Ethernet2.h> //pre Wiznet W5500 Ethernet shield
#elif defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>

#if defined(ESP32) || defined(ESP8266)
WiFiClient espClient;
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
EthernetClient espClient;
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x81, 0x7B, 0x4A };
//IPAddress ip(192, 168, 0, 2);
//IPAddress dnServer(192, 168, 0, 1);
//IPAddress gateway(192, 168, 0, 1);
//IPAddress subnet(255, 255, 255, 0);
#endif
const char* mqtt_server = "mqtt.iotindustries.sk";
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
  Serial.println("UART ready");
#if defined(ESP32) || defined(ESP8266)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac);
    //Ethernet.begin(mac, ip);
    //Ethernet.begin(mac, ip, dns);
    //Ethernet.begin(mac, ip, dns, gateway);
    //Ethernet.begin(mac, ip, dns, gateway, subnet);
  }
  Serial.print("  DHCP assigned IP ");
  Serial.println(Ethernet.localIP());
#endif
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
#if defined(ESP32) || defined(ESP8266)
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if (Ethernet.begin(mac) == 0) {
    Ethernet.begin(mac);
    //Ethernet.begin(mac, ip);
    //Ethernet.begin(mac, ip, dns);
    //Ethernet.begin(mac, ip, dns, gateway);
    //Ethernet.begin(mac, ip, dns, gateway, subnet);
  }
#endif
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
