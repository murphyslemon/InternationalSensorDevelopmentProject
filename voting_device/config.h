#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>

#include "power.h"
#include "display.h"

//#include <Wifi.h>

#define STRINGSIZE          256
#define BATTERY_PIN         A0
#define REFERENCE_VOLTAGE   3.3

#include "mqtt.h"

//Definition of GPIOS for Buttons
#define BUTTON_PIN_1 0   // GPIO 5 (entspricht D1) für Taster 1
#define BUTTON_PIN_2 2   // GPIO 4 (entspricht D2) für Taster 2
#define BUTTON_PIN_3 12  // GPIO 16 (entspricht D0) für Taster 3

#define BOOT        0
#define QUESTION    1
#define VOTE        2
#define CONFIRM     3
#define CLOSE_VOTE  4

#define MQTTpubQos 1                  
#define MQTTsubQos 1

//configuration of functionality
//#define ENCRYPTION

//Includes according to config
#ifdef ENCRYPTION
#include <encryptionlibrary.h>  //todo find encryption library
#endif

#ifdef ISRS_FOR_BUTTONS
#include "button_interrupts.h"
void attachISR(void){
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), Isr_Btn_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), Isr_Btn_2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_3), Isr_Btn_3, FALLING);
}
#endif
#endif
