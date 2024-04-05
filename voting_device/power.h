#ifndef POWER
#define POWER
#include <Arduino.h>
#include "display.h"
#include "config.h"
#define RX_PIN    3
#define PWR_PIN   5

//Battery
#define FULL_BATTERY    28
#define EMPTY_BATTERY   4

void powerOff();

int checkBatteryLevel();

#endif