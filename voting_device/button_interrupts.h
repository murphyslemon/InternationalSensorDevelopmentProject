#ifndef BUTTON_ISR
#define BUTTON_ISR
#include <Arduino.h>

#include "Button_Class.h"
extern ButtonVote ButtonYes;
extern ButtonVote ButtonNo;
extern ButtonVote ButtonAbstain;


ICACHE_RAM_ATTR void Isr_Btn_1(void);
ICACHE_RAM_ATTR void Isr_Btn_2(void);
ICACHE_RAM_ATTR void Isr_Btn_3(void);


#endif