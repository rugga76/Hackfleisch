#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*Constants for WIFI*/
const char*    SSID             = "EinarSchiessHier";
const char*    PSK              = "LetMeIn";

/*Constants for MQTT*/
const char*    MQTT_BROKER_IP   = "192.168.0.255";
const uint16_t MQTT_BROKER_PORT = 1883;
const char*    MQTT_USERNAME    = "Haxxor";
const char*    MQTT_PASSWORD    = "ByteMe";

/*Constant Pin Mapping of the NodeMCU Pin Naming to Arduino GPIO Naming*/
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
