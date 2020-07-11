//--------------------------------------------------------------------------------
// S字曲線（シグモイド関数：sigmoid function）で動かしてみました　by aya
// http://maison-dcc.sblo.jp/ http://dcc.client.jp/ https://twitter.com/masashi_214
//--------------------------------------------------------------------------------

#include <Servo.h>
#include <math.h>
#include "SigmoidFunction.h"
//各種設定、宣言

#define PIN_F0_F  3     // D3 PD3,PWM
#define PIN_F0_R  4     // D4 PD4
#define PIN_AUX1  5     // D5 PD5
#define PIN_AUX2  6     // D6 PD6
#define PIN_AUX3  7     // D7 PD7
#define PIN_AUX4  8     // D8 PB0
#define PIN_AUX5  9     // D9 PB1,DIGITAL TR,PWM
#define PIN_AUX6  10    // D10 PB2,DIGITAL TR,PWM
#define PIN_AUX7  11    // D11 PB3,DIGITAL TR,PWM

#define MAX_SERVO 2     // サーボの最大数2個
#define PIN_SERVO1 5    // D5 PD5
#define PIN_SERVO2 4    // D4 PD4
#define PIN_LED_DIV1 6  // D6 PD6 SERVO1用DIV(分岐) LED
#define PIN_LED_STR1 7  // D7 PD7 SERVO1用STR(直進) LED
#define PIN_LED_DIV2 8  // D8 PB0 SERVO2用DIV(分岐) LED
#define PIN_LED_STR2 9  // D9 PB1 SERVO2用STR(直進) LED

#define ANGLE_MIN_A 670     // DM-S0025 400-2100  0-180deg
#define ANGLE_MAX_A 2600     // DM-S0037 1000-2000 0-90deg 
                             // DM-S0037 700-2400 0-180deg
                             
#define ANGLE_MIN_B 500     // 1000 -90deg
#define ANGLE_MAX_B 2000    // 2000 +90deg

unsigned long PreviosTime = 0;
unsigned long PreviosTimeState = 0;

void setup() {
  Serial.begin(115200);
  PreviosTime = millis();
  PreviosTimeState = PreviosTime;
}

void loop() {
  static SigmoidFunction ServoA(0,PIN_SERVO1,670,2600,1);
  static SigmoidFunction ServoB(1,PIN_SERVO2,400,2100,1);
  static int state = 0;
  int ret;
  
  if( (millis() - PreviosTime ) >= 10 ){  // 1000:1000msec 前回から1000ms経過したかチェック
    PreviosTime = millis();

    switch(state){
      case 0:
              ServoA.exec(0,70);
              ServoB.exec(0,70);
              ServoA.exec(550,0);
              ServoB.exec(550,0);
             state = 1;
             break;
      case 1:
              ret = ServoA.stateCheck();
              ret = ServoB.stateCheck();
              if(ret==3)
                state = 2;
              break;
      case 2:
              ServoA.exec(500,70);
              ServoB.exec(500,70);
              state = 3;
              break;
      case 3:
              ret = ServoA.stateCheck();
              ret = ServoB.stateCheck();
             break;            
      default:
             break;
    }
  }

  if( (millis() - PreviosTimeState ) >= 1000 ){  // 1000:1000msec 前回から1000ms経過したかチェック
    PreviosTimeState = millis();
    Serial.println(ServoA.nowState());
  }
}
