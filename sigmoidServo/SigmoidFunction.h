#ifndef SIGMOIDFUNCTION_H_
#define SIGMOIDFUNCTION_H_

#include <Servo.h>

// 状態基底クラス
class SigmoidFunction
{
public:
  SigmoidFunction(char ch,unsigned char port, int amin, int amax, int gain);
  int stateCheck();
  void servoABwite(char ch, int ref);
  int nowState();
  void exec(int tim, int deg);

private:
  char state = ST_INIT;
  char updownFlg;
  char svch;
  
  unsigned char port;

  float PwmRef;
  float deltPwm;            // 10msあたりのpwm増加量
  float lmax;
  float deltSig;
  float SigmoidCurve;
  float ServoDeg;
  int nowPwm;
  int nextPwm;
  int minAngle;
  int maxAngle;
  int tablek;
  int pos;

  //S字曲線（シグモイド曲線）
  float SigmoidCurveTable[10+1]={0,5,2.6,1.8,1.3,1,0.8,0.7,0.6,0.5,0.5};
  
  Servo   ServoA;  // create servo object to control a servo
  Servo   ServoB;  // create servo object to control a servo

  enum{
    ST_INIT = 0,
    ST_DOWN,
    ST_UP,
    ST_END,
  };

  enum{
    DOWN = 0,
    STY,
    UP,
  };
  


};

#endif
