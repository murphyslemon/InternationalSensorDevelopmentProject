#include "button_interrupts.h"

ButtonVote ButtonYes;
ButtonVote ButtonNo;
ButtonVote ButtonAbstain;

ICACHE_RAM_ATTR void Isr_Btn_1(void){
  delay(30);// Debounce
  ButtonYes.setState(1);
//todo send stuff if button 1 is pressed
}
ICACHE_RAM_ATTR void Isr_Btn_2(void){
  delay(30);// Debounce
  ButtonAbstain.setState(1);
//todo send stuff if button 2 is pressed
}
ICACHE_RAM_ATTR void Isr_Btn_3(void){
  delay(30);// Debounce
  ButtonNo.setState(1);
//todo send stuff if button 3 is pressed
}

