WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

int inputvalueGateIsInPositionClosed      = 2;
int inputvalueGateIsInPositionOpened      = 2;
int inputvalueGateIsInPositionVentilation = 2;

void setup() {
  pinMode(D0, OUTPUT); //GPIO16 = D0 MoveToGatePositionClosed
  pinMode(D1, OUTPUT); //GPIO5  = D1 MoveToGatePositionOpened
  pinMode(D2, OUTPUT); //GPIO4  = D2 MoveToGatePositionVentilation
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D5, INPUT_PULLUP); //GPIO14 = D5
  pinMode(D6, INPUT_PULLUP); //GPIO12 = D6
  pinMode(D7, INPUT);        //GPIO13 = D7
  //pinMode(D8, INPUT_PULLUP); //GPIO15 = D8
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
  client.setCallback(mqtt_callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PSK);
  WiFi.hostname("ESP8266Supramatic");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
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
  //Serial.println(msg);
  
  /*process the topic and message*/
  if      (strcmp(topic, "ZWay/Garage/GateMoveToPositionClosed") == 0) {
    /*Topic is for PIN D0*/
    if (strcmp(msg, "on") == 0) {
      digitalWrite(D0, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      client.publish("ZWay/Garage/GateMoveToPositionClosed/set", "off");
    }
    else if (strcmp(msg, "off") == 0) {
      digitalWrite(D0, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else if (strcmp(topic, "ZWay/Garage/GateMoveToPositionOpened") == 0) {
    /*Topic is for PIN D1*/
    if (strcmp(msg, "on") == 0) {
      digitalWrite(D1, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      client.publish("ZWay/Garage/GateMoveToPositionOpened/set", "off");
    }
    else if (strcmp(msg, "off") == 0) {
      digitalWrite(D1, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else if (strcmp(topic, "ZWay/Garage/GateMoveToPositionVentilation") == 0) {
    /*Topic is for PIN D2*/
    if (strcmp(msg, "on") == 0) {
      digitalWrite(D2, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      client.publish("ZWay/Garage/GateMoveToPositionVentilation/set", "off");
    }
    else if (strcmp(msg, "off") == 0) {
      digitalWrite(D2, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else if (strcmp(topic, "ZWay/Garage/GateIsInPositionClosed") == 0) {
    if ((strcmp(msg, "on")  == 0 and inputvalueGateIsInPositionClosed == 1) or
        (strcmp(msg, "off") == 0 and inputvalueGateIsInPositionClosed == 0)){
        Serial.println("Mismatch between ZWay Status for GateIsInPositionClosed and Pin Reading");
    }
  }
  else if (strcmp(topic, "ZWay/Garage/GateIsInPositionOpened") == 0) {
    if ((strcmp(msg, "on")  == 0 and inputvalueGateIsInPositionOpened == 1) or
        (strcmp(msg, "off") == 0 and inputvalueGateIsInPositionOpened == 0)){
        Serial.println("Mismatch between ZWay Status for GateIsInPositionOpened and Pin Reading");
    }
  }
  else if (strcmp(topic, "ZWay/Garage/GateIsInPositionVentilation") == 0) {
    if ((strcmp(msg, "on")  == 0 and inputvalueGateIsInPositionVentilation == 0) or
        (strcmp(msg, "off") == 0 and inputvalueGateIsInPositionVentilation == 1)){
        Serial.println("Mismatch between ZWay Status for GateIsInPositionVentilation and Pin Reading");
    }
  }
  else {
    Serial.print("unmatched topic ");
    Serial.println(topic);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting MQTT...");
    if (!client.connect("ESP8266_Supramatic", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
    }
  }
  
  client.subscribe("ZWay/Garage/GateMoveToPositionClosed");
  client.subscribe("ZWay/Garage/GateMoveToPositionOpened");
  client.subscribe("ZWay/Garage/GateMoveToPositionVentilation");
  client.subscribe("ZWay/Garage/GateIsInPositionClosed");
  client.subscribe("ZWay/Garage/GateIsInPositionOpened");
  client.subscribe("ZWay/Garage/GateIsInPositionVentilation");
  
  Serial.println("MQTT Connected...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  //Serial.println("Loop Start");

  int tmpvarD5 = digitalRead(D5);
  int tmpvarD6 = digitalRead(D6);
  int tmpvarD7 = digitalRead(D7);

  if (inputvalueGateIsInPositionClosed != tmpvarD5)
  {
    inputvalueGateIsInPositionClosed = tmpvarD5;
    Serial.print("Input D5 inputvalueGateIsInPositionClosed has changed to value ");
    Serial.println(inputvalueGateIsInPositionClosed);

    if (inputvalueGateIsInPositionClosed == 0)
    {
      client.publish("ZWay/Garage/GateIsInPositionClosed/set", "on");
    }
    else
    {
      client.publish("ZWay/Garage/GateIsInPositionClosed/set", "off");
    }
  };

  if (inputvalueGateIsInPositionOpened != tmpvarD6)
  {
    inputvalueGateIsInPositionOpened =  tmpvarD6;
    Serial.print("Input D6 inputvalueGateIsInPositionOpened has changed to value ");
    Serial.println(inputvalueGateIsInPositionOpened);

    if (inputvalueGateIsInPositionOpened == 0)
    {
      client.publish("ZWay/Garage/GateIsInPositionOpened/set", "on");
    }
    else
    {
      client.publish("ZWay/Garage/GateIsInPositionOpened/set", "off");
    }
  }

  if (inputvalueGateIsInPositionVentilation != tmpvarD7)
  {
    inputvalueGateIsInPositionVentilation =  tmpvarD7;
    Serial.print("Input D7 inputvalueGateIsInPositionVentilation has changed to value ");
    Serial.println(inputvalueGateIsInPositionVentilation);

    if (inputvalueGateIsInPositionVentilation == 1)
    {
      client.publish("ZWay/Garage/GateIsInPositionVentilation/set", "on");
    }
    else
    {
      client.publish("ZWay/Garage/GateIsInPositionVentilation/set", "off");
    }
  }
  delay(100);

  client.loop();
}
