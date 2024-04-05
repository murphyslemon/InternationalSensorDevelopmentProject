#include "config.h"

//#################################################################################################
//### REMEMBER TO COMMENT OUT: Serial.begin(115200); FROM SETUP FUNCTION, AS IT AFFECTS RX PIN. ###
//#################################################################################################

// WLAN-Settings
  const char* ssid = "ISDProjectLAN";
  const char* password = "557949122";

//Mqtt topics to publish
  const String pubInit = "/registration/Server/"+ String(WiFi.macAddress());
  const char* pubPubVote = "/vote/";

// MQTT topics to subscribe
  const String subInit = "/registration/esp/"+ String(WiFi.macAddress()); // /registration/esp/
  const char* subVoteSetup = "/setupVote/Setup";
  const char* subResync = "/setupVote/Resync";

//battery level
  int batteryPercentage = checkBatteryLevel();

//program variables
  int state = 0;
  char response[STRINGSIZE] = "";
  char votingID[STRINGSIZE] = "";
  char pubTopicVoteResponse[STRINGSIZE] = "";
  char voteTitle[STRINGSIZE] = "";
  char responseBuffer[STRINGSIZE+50];

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  //Serial.begin(115200);

//button initialization
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);  // Taster 1 als Eingang mit Pull-up-Widerstand
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);  // Taster 2 als Eingang mit Pull-up-Widerstand
  pinMode(PWR_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT_PULLUP);
  digitalWrite(PWR_PIN, HIGH); //power on device
  
//Display initialization
  initDisplay();
  startupScreen(batteryPercentage);

//wifi initialization
  WiFi.begin(ssid, password);
  connectToWiFi();

//mqtt initialization
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
  connectToMQTT();
  String macAddress = "{\"Mac\" : \"" + String(WiFi.macAddress()) + "\"}";
  Serial.println(macAddress);
  mqttClient.subscribe(subInit.c_str(), MQTTsubQos); //recieve voting ID
  mqttClient.publish(pubInit.c_str(), macAddress.c_str());  //send mac address
  Serial.println("#" + String(subInit.c_str()) + "#") ;
  //mqttClient.subscribe(subResync, MQTTsubQos);
  mqttClient.subscribe(subVoteSetup, MQTTsubQos); //recieve question
}
  
void loop() {
  powerOff();
  switch (state) {
    case BOOT:
  //display start up screen
      if (MQTT_flag) {
        MQTT_flag = 0;
        strcpy(votingID, MQTTVotingId);
        Serial.println(votingID);
        state = QUESTION; 
      }
      break;

    case QUESTION:
      if (MQTT_flag) {
        MQTT_flag = 0;
        strcpy(voteTitle, MQTTVotingTitle);
        Serial.println(voteTitle);
        paintVoteScreen(voteTitle);
        strcat(pubTopicVoteResponse, pubPubVote);
        strcat(pubTopicVoteResponse, votingID);
        Serial.println(pubTopicVoteResponse);
        state = VOTE;
      }
      break;

    case VOTE:
      if (!digitalRead(BUTTON_PIN_1)) {
        delay(30);
        if (!digitalRead(BUTTON_PIN_1)){
          Serial.println("I am in Button1: Vote");
          snprintf(responseBuffer, sizeof(responseBuffer), "{\"vote\":\"Yes\", \"VoteTitle\":\"%s\"}", MQTTVotingTitle);
          strcpy(response, responseBuffer);
          paintConfirmScreen("YES");
          Serial.println("YES");
          state = CONFIRM;
          while (!digitalRead(BUTTON_PIN_1)){
            delay(100);
          }
        }      
      }
      else if (!digitalRead(BUTTON_PIN_2)) {
        delay(30);
        if (!digitalRead(BUTTON_PIN_2)){
          snprintf(responseBuffer, sizeof(responseBuffer), "{\"vote\":\"Abstain\", \"VoteTitle\":\"%s\"}", MQTTVotingTitle);
          strcpy(response, responseBuffer);
          paintConfirmScreen("ABSTAIN");
          Serial.println("ABSTAIN");
          state = CONFIRM;
          while (!digitalRead(BUTTON_PIN_2)){
            delay(100);
          }
        }
      }
      else if (!digitalRead(BUTTON_PIN_3)) {
        delay(30);
        if (!digitalRead(BUTTON_PIN_3)){
          snprintf(responseBuffer, sizeof(responseBuffer), "{\"vote\":\"No\", \"VoteTitle\":\"%s\"}", MQTTVotingTitle);
          strcpy(response, responseBuffer);
          paintConfirmScreen("NO");
          Serial.println("NO");
          state = CONFIRM;
          while (!digitalRead(BUTTON_PIN_3)){
            delay(100);
          }
        }
      }
      break;

    case CONFIRM:
    // ButtonYes.getState()
      if (!digitalRead(BUTTON_PIN_1)) {
        delay(30);
        if (!digitalRead(BUTTON_PIN_1)){
          Serial.println("I am in Button1: Confirm");
          mqttClient.publish(pubTopicVoteResponse, response);
          state = CLOSE_VOTE;
          while (!digitalRead(BUTTON_PIN_1)){
            delay(100);
          }
        }
      }
      else if (!digitalRead(BUTTON_PIN_3)) {
        delay(30);
        if (!digitalRead(BUTTON_PIN_3)){
          paintVoteScreen(voteTitle);
          state = VOTE;
          while (!digitalRead(BUTTON_PIN_3)){
            delay(100);
          }
        }
      }
      break;

    case CLOSE_VOTE:
      delay(2000);
      paintClosingScreen();
      state = QUESTION;
      break;
  }

 mqttClient.loop();
}
