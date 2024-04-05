#include "Button_Class.h"

bool ButtonVote::getState(void){
  bool temp =ButtonVote::State;
  ButtonVote::resetState();
  return temp;
}

void ButtonVote::setState(bool State){
  ButtonVote::State = State;
}

void ButtonVote::resetState(void){
  ButtonVote::State = false;
}