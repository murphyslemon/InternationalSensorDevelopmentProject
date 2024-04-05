#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>  // default wifi library
#include <PubSubClient.h> //Mqtt library by Nick O'Leary

#include "config.h"

// MQTT-Client erstellen
  extern WiFiClient wifiClient;
  extern PubSubClient mqttClient;
  extern const size_t JSON_CAPACITY;
  extern char MQTTmsg[STRINGSIZE];
  extern bool MQTT_flag;
  extern char MQTTVotingId [STRINGSIZE];
  extern char MQTTVotingTitle [STRINGSIZE];

  // MQTT-Server Settings
  extern const char* mqtt_server;
  extern const int mqtt_port;
  extern const char* mqtt_user;
  extern const char* mqtt_password;

void callback(char* topic, byte* payload, unsigned int length);

void connectToMQTT();

#endif