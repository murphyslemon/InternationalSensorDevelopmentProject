#ifndef BUTTONCLASS
#define BUTTONCLASS

class ButtonVote{
public:
  bool getState(void);
  void setState(bool);
private:
  bool State = false;
  void resetState(void);
};


#endif