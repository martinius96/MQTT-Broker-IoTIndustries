//ESP32, ESP8266 - Publish / Subscribe - MQTTS
//Author: Martin Chlebovec (martinius96)
//ESP8266 TEST CORE: 3.0.2
//ESP32 TEST CORE: 2.0.1
//Donate for more: https://paypal.me/chlebovec

#include <PubSubClient.h>
#include <WiFiClientSecure.h>
const char* ssid = "MENO_WIFI";
const char* password = "HESLO_WIFI";

// Root CA
const static char* test_root_ca PROGMEM = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIFdzCCBF+gAwIBAgIQE+oocFv07O0MNmMJgGFDNjANBgkqhkiG9w0BAQwFADBv\n" \
    "MQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFk\n" \
    "ZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBF\n" \
    "eHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFow\n" \
    "gYgxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpOZXcgSmVyc2V5MRQwEgYDVQQHEwtK\n" \
    "ZXJzZXkgQ2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMS4wLAYD\n" \
    "VQQDEyVVU0VSVHJ1c3QgUlNBIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjAN\n" \
    "BgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAgBJlFzYOw9sIs9CsVw127c0n00yt\n" \
    "UINh4qogTQktZAnczomfzD2p7PbPwdzx07HWezcoEStH2jnGvDoZtF+mvX2do2NC\n" \
    "tnbyqTsrkfjib9DsFiCQCT7i6HTJGLSR1GJk23+jBvGIGGqQIjy8/hPwhxR79uQf\n" \
    "jtTkUcYRZ0YIUcuGFFQ/vDP+fmyc/xadGL1RjjWmp2bIcmfbIWax1Jt4A8BQOujM\n" \
    "8Ny8nkz+rwWWNR9XWrf/zvk9tyy29lTdyOcSOk2uTIq3XJq0tyA9yn8iNK5+O2hm\n" \
    "AUTnAU5GU5szYPeUvlM3kHND8zLDU+/bqv50TmnHa4xgk97Exwzf4TKuzJM7UXiV\n" \
    "Z4vuPVb+DNBpDxsP8yUmazNt925H+nND5X4OpWaxKXwyhGNVicQNwZNUMBkTrNN9\n" \
    "N6frXTpsNVzbQdcS2qlJC9/YgIoJk2KOtWbPJYjNhLixP6Q5D9kCnusSTJV882sF\n" \
    "qV4Wg8y4Z+LoE53MW4LTTLPtW//e5XOsIzstAL81VXQJSdhJWBp/kjbmUZIO8yZ9\n" \
    "HE0XvMnsQybQv0FfQKlERPSZ51eHnlAfV1SoPv10Yy+xUGUJ5lhCLkMaTLTwJUdZ\n" \
    "+gQek9QmRkpQgbLevni3/GcV4clXhB4PY9bpYrrWX1Uu6lzGKAgEJTm4Diup8kyX\n" \
    "HAc/DVL17e8vgg8CAwEAAaOB9DCB8TAfBgNVHSMEGDAWgBStvZh6NLQm9/rEJlTv\n" \
    "A73gJMtUGjAdBgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/\n" \
    "BAQDAgGGMA8GA1UdEwEB/wQFMAMBAf8wEQYDVR0gBAowCDAGBgRVHSAAMEQGA1Ud\n" \
    "HwQ9MDswOaA3oDWGM2h0dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9BZGRUcnVzdEV4\n" \
    "dGVybmFsQ0FSb290LmNybDA1BggrBgEFBQcBAQQpMCcwJQYIKwYBBQUHMAGGGWh0\n" \
    "dHA6Ly9vY3NwLnVzZXJ0cnVzdC5jb20wDQYJKoZIhvcNAQEMBQADggEBAJNl9jeD\n" \
    "lQ9ew4IcH9Z35zyKwKoJ8OkLJvHgwmp1ocd5yblSYMgpEg7wrQPWCcR23+WmgZWn\n" \
    "RtqCV6mVksW2jwMibDN3wXsyF24HzloUQToFJBv2FAY7qCUkDrvMKnXduXBBP3zQ\n" \
    "YzYhBx9G/2CkkeFnvN4ffhkUyWNnkepnB2u0j4vAbkN9w6GAbLIevFOFfdyQoaS8\n" \
    "Le9Gclc1Bb+7RrtubTeZtv8jkpHGbkD4jylW6l/VXxRTrPBPYer3IsynVgviuDQf\n" \
    "Jtl7GQVoP7o81DgGotPmjw7jtHFtQELFhLRAlSv0ZaBIefYdgWOWnU914Ph85I6p\n" \
    "0fKtirOMxyHNwu8=\n" \
    "-----END CERTIFICATE-----\n";
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
X509List cert(test_root_ca);
#endif
#if defined(ESP32) || defined(ESP8266)
WiFiClientSecure  espClient;
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
#if defined(ESP32)
  espClient.setCACert(test_root_ca);
      WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
#elif defined(ESP8266)
      WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
  Serial.printf("Using certificate: %s\n", test_root_ca);
  client.setTrustAnchors(&cert);
#endif
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 8883);
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
