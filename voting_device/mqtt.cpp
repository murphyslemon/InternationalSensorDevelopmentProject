#include "mqtt.h"

// MQTT-Server Settings
  const char* mqtt_server = "10.42.0.1";
  const int mqtt_port = 1883;
  const char* mqtt_user = "";
  const char* mqtt_password = "";

// MQTT-Client erstellen
  WiFiClient wifiClient;
  PubSubClient mqttClient(wifiClient);
  const size_t JSON_CAPACITY = 256;
  char MQTTmsg[STRINGSIZE] = "";
  bool MQTT_flag = 0;
  char MQTTVotingId [STRINGSIZE]= "";
  char MQTTVotingTitle [STRINGSIZE]= "";

// MQTT Callback Function
void callback(char* topic, byte* payload, unsigned int length) {
  // Copy payload bytes to MQTTmsg
  memcpy(MQTTmsg, payload, length);
  MQTTmsg[length] = '\0'; // Ensure null-termination for string
  Serial.println(MQTTmsg);
  // Parse JSON
  StaticJsonDocument<JSON_CAPACITY> doc;
  DeserializationError error = deserializeJson(doc, MQTTmsg);
  
  // If parsing succeeds, extract the necessary fields
  if (!error) {
    if (doc.containsKey("VotingID")) {
      const char* votingID = doc["VotingID"];
      Serial.print("Received Voting ID: ");
      Serial.println(votingID);
      memcpy(MQTTVotingId, votingID, strlen(votingID) + 1);
      // Optionally, you can set a flag to indicate that a new ID has been received
      MQTT_flag = true;
    }
    
    if (doc.containsKey("VoteTitle")) {
      const char* voteTitle = doc["VoteTitle"];
      Serial.print("Received Vote Title: ");
      Serial.println(voteTitle);
      memcpy(MQTTVotingTitle, voteTitle, strlen(voteTitle) + 1);
      // Optionally, you can set a flag to indicate that a new title has been received
      MQTT_flag = true;
    }
    
    // Add more conditions to extract other fields as needed
  } else {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
  }
}

void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to the MQTT server");
    mqttClient.connect(WiFi.macAddress().c_str(), mqtt_user, mqtt_password);
    delay(500);
  }
  Serial.println("Connection to MQTT server established");
}