WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(D1, OUTPUT); //GPIO5  = D1
  pinMode(D2, OUTPUT); //GPIO4  = D2
  pinMode(D7, OUTPUT); //GPIO13 = D7
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PSK);
  WiFi.hostname("HalloweenGhost");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  /*Print Topic (already string), then convert and print bytes-payload to msg-string*/
  Serial.print("Received message [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i] = (char)payload[i];
  }
  Serial.println();
  msg[length] = '\0';
  Serial.println(msg);

  /*process the topic and message*/
  if      (strcmp(topic, "ZWay/Halloween/Ghost/D1") == 0) {
    /*Topic is for PIN D1*/
    if (strcmp(msg, "on") == 0) {
      digitalWrite(D1, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (strcmp(msg, "off") == 0) {
      digitalWrite(D1, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else if (strcmp(topic, "ZWay/Halloween/Ghost/D2") == 0) {
    /*Topic is for PIN D2*/
    if (strcmp(msg, "on") == 0) {
      digitalWrite(D2, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (strcmp(msg, "off") == 0) {
      digitalWrite(D2, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else 
  {
    Serial.print("unmatched topic ");
    Serial.print(topic);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting MQTT...");
    if (!client.connect("ESP8266_Ghost", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
    }
  }
  client.subscribe("/ZWay/Halloween/Ghost/D1");
  client.subscribe("/ZWay/Halloween/Ghost/D2");
  Serial.println("MQTT Connected...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
