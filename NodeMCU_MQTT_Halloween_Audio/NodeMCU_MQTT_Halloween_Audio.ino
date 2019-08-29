#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Wtv020sd16p.h>

#ifndef STASSID
#define STASSID "EinarSchiessHier";
#define STAPSK  "LetMeIn";
#endif

/*Constants for WIFI*/
const char* SSID          = STASSID;
const char* PSK           = STAPSK;

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

/*WTV020-SD-16P Audio Board Pinning*/
int resetPin = 0;  // The pin number of the reset pin.
int clockPin = 2;  // The pin number of the clock pin.
int dataPin  = 14;  // The pin number of the data pin.
int busyPin  = 12;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  //Define LED-Output Pin
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Initialize Debug Serial Console
  Serial.begin(115200);
  Serial.println("Booting");
  
  //Connect to WIFI
  setup_wifi();
  
  //Over-the-Air-Update Code
  //setup_ota();

  //Set MQTT-Broker Settings
  client.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
  client.setCallback(mqtt_callback);

  //Resetting WTV020sd16p, suitable PIN-Modes are being set in this routine
  Serial.println("Resetting WTV020sd16p...");
  wtv020sd16p.reset();

  Serial.println("Setting Volume 1");
  wtv020sd16p.setVolume(1);

  Serial.println("Audiobox Ready");
  wtv020sd16p.asyncPlayVoice(0);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WIFI ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PSK);
  WiFi.hostname("AudioBox1");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*void setup_ota() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  
  Serial.println("Setup() completed, ready");
}*/
