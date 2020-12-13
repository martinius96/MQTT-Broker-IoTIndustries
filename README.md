# MQTT Broker - IoT Industries Slovakia
* **Server:** mqtt.iotindustries.sk
* **MQTTPort:** 1883
* **Secured MQTT Port:** 8883 (Working only for ESP8266, ESP32)
* Example for Arduino, ESP8266, ESP32 boards
* **Copy PubSubClient to your Arduino/libraries directory**
* Directory is at: C:/Users/User/Documents/ - Win 10 example
* Plug n' Play implementations
#
![MQTT Broker - UART - Arduino + Ethernet W5100](https://i.imgur.com/DyHkXkt.png)
# GET ROOT CA (FOR ESP32 - MQTTS CONENCTION)
* openssl s_client -showcerts -verify 5 -connect mqtt.iotindustries.sk:8883 < /dev/null
# GET FINGERPRINT (FOR ESP8266 - MQTTS CONENCTION)
* openssl s_client -connect mqtt.iotindustries.sk:8883 -showcerts < /dev/null 2>/dev/null   | openssl x509 -in /dev/stdin -sha1 -noout -fingerprint
