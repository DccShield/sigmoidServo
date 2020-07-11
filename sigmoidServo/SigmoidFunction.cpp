//------------------------------------------------------------------------------
// ServoSequenceクラス
//------------------------------------------------------------------------------
#include <arduino.h>
#include "SigmoidFunction.h"

// コンストラクタ
SigmoidFunction::SigmoidFunction(char ch,unsigned char port, int amin, int amax, int gain)
{
  pinMode(port, OUTPUT);
  digitalWrite(port, HIGH);

  minAngle = amin;
  maxAngle = amax;
  tablek  = gain;
  
  if(ch == 0)
    ServoA.attach(port, minAngle, maxAngle);
  if(ch == 1)
    ServoB.attach(port, minAngle, maxAngle);
    
  svch = ch;
  state = ST_INIT;
  lmax = SigmoidCurveTable[gain];
}

int SigmoidFunction::nowState()
{
  return state; 
}

void SigmoidFunction::exec(int tim, int deg)
{
  if(tim == 0){
    nowPwm =  map(deg,0,180,minAngle,maxAngle);
    servoABwite(svch, (int)nowPwm);
  }
  nextPwm = map(deg,0,180,minAngle,maxAngle);
  deltSig = (nextPwm - nowPwm) / (float)tim /100.0;
  state = ST_INIT;
}

void SigmoidFunction::servoABwite(char ch, int ref)
{
  if(ch == 0){
    ServoA.writeMicroseconds(ref);
  }
  if(ch == 1){
    ServoB.writeMicroseconds(ref);
  }
}


// SigmoidFunction ステートマシン（状態遷移）
int SigmoidFunction::stateCheck()
{
  switch(state){
    case ST_INIT:
                  if(nextPwm - nowPwm < 0){
                    SigmoidCurve = lmax;
                    state = ST_DOWN;
                  } else {
                    SigmoidCurve = -lmax;
                    state = ST_UP;
                  }                    
                  break;
    case ST_UP:
                  SigmoidCurve = SigmoidCurve + deltSig;
                  if( SigmoidCurve > lmax){
                    servoABwite(svch, (int)nextPwm);
                    nowPwm = nextPwm;
                    state = ST_END;
                    break;
                  }
                  ServoDeg = 2000 * (1/ (1 + exp(-tablek*SigmoidCurve) ) );
                  pos = map(ServoDeg,0,2000,nowPwm,nextPwm);
                  servoABwite(svch, (int)pos);  
                  break;

    case ST_DOWN:
                  SigmoidCurve = SigmoidCurve + deltSig;
                  if( SigmoidCurve < -lmax){
                    servoABwite(svch, (int)nextPwm);
                    nowPwm = nextPwm;
                    state = ST_END;
                    break;
                  }
                  ServoDeg = 2000 * (1/ (1 + exp(-tablek*SigmoidCurve) ) );
                  pos = map(ServoDeg,2000,0,nowPwm,nextPwm);
                  servoABwite(svch, (int)pos);  
                  break;
                  
    case ST_END:
                  break;

    default:
                  break;
  }
  return state;
}
