
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
  Serial.println(msg);

  /*process the topic and message*/
  if      (strcmp(topic, "ZWay/Halloween/Audiobox1/Audioindex") == 0) {
    /*Topic is for this AudioBox*/
    int audioindex = atoi(msg);
    if     (audioindex == 0) {
      Serial.println("Stopping AsyncAudio");
      wtv020sd16p.stopVoice();
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (audioindex > 0 && audioindex < 8) {
      Serial.print("Setting Volume: ");
      Serial.println(audioindex);
      wtv020sd16p.setVolume(audioindex);
    }
    else
    {
       Serial.print("Playing Audioindex: ");
       Serial.println(audioindex);
       wtv020sd16p.asyncPlayVoice(audioindex);
       digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else {
    Serial.print("Unmatched topic: ");
    Serial.println(topic);
  }
}

void mqtt_reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting MQTT...");
    if (!client.connect("ESP8266_Ghost", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
    }
  }
  client.subscribe("/ZWay/Halloween/Audiobox1/Audioindex");
  Serial.println("MQTT Connected...");
}

void loop() {
  /*OTA Update Handle*/
  //ArduinoOTA.handle();

  /*Serial.println("loop test");
  wtv020sd16p.setVolume(1);
  
  Serial.println("Playing Index 25");
  wtv020sd16p.asyncPlayVoice(25);  
  delay(12000);

  Serial.println("Playing Index 20");
  wtv020sd16p.asyncPlayVoice(20);  
  delay(12000);

  Serial.println("Playing Index 21");
  wtv020sd16p.asyncPlayVoice(21);  
  delay(12000);

  Serial.println("Playing Index 19");
  wtv020sd16p.asyncPlayVoice(19);  
  delay(8000);

  Serial.println("Playing Index 14");
  wtv020sd16p.asyncPlayVoice(14);  
  delay(8000);

  Serial.println("Playing Index 18");
  wtv020sd16p.asyncPlayVoice(18);  
  delay(8000);*/

  /*MQTT Client Connection*/
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop();
  delay(40);
}
