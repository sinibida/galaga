#ifndef IO_H
#define IO_H

#include "constants.h"
#include "structs.h"

namespace io { // 입출력 관련 함수들
  int pinNum[] = { // led 핀 배열
    PIN_LED_1,
    PIN_LED_2,
    PIN_LED_3,
  };

  void init() {
    for(int i = 0; i < 3; i++) { // led 핀 초기화
      pinMode(pinNum[i], OUTPUT);
    }
    pinMode(PIN_SOUND_BUZZER_1, OUTPUT);
    pinMode(PIN_SOUND_BUZZER_2, OUTPUT);
    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);
    pinMode(PIN_LED_3, OUTPUT);

    pinMode(PIN_BUTTON_SHOOT, INPUT); // 버튼 핀 초기화
    pinMode(PIN_BUTTON_LEFT, INPUT);
    pinMode(PIN_BUTTON_RIGHT, INPUT);
  }

  //led 핀의 배열과 life를 받음
  void lifeViewer(int life){
    for (int i = 0; i < 3; i++) {
      if (i < life) {
        digitalWrite(pinNum[2-i], HIGH);
      } else {
        digitalWrite(pinNum[2-i], LOW);
      }
    }
  }

  int getStart() { // SHOOT 버튼 누른 경우 1 반환
    return digitalRead(PIN_BUTTON_SHOOT) == HIGH;
  }

  bool lastL = false;
  int getRight() {
    int left = digitalRead(PIN_BUTTON_RIGHT);
    int down = left == HIGH && !lastL;
    lastL = left;
    return down;
  }

  bool lastShootInput = false;
  unsigned long long lastShootDown = millis();
  Timer burstTimer = {250};
  // 총 발사 버튼 입력 받음
  int getShoot() {
    int shoot = digitalRead(PIN_BUTTON_SHOOT);
    int down = shoot == HIGH && !lastShootInput;
    lastShootInput = shoot;

    if (down) {
      lastShootDown = millis();
    }

    if (!shoot) {
      return 0;
    }

    if (millis() > lastShootDown + 750) { // 연사 시작
      if (burstTimer.lastT + burstTimer.interval < millis()) {
        burstTimer.lastT = millis();
        return 2;
      }
      return 0;
    } else {
      return down;
    }
  }

  bool lmove = false;
  bool rmove = false;
  // 원쪽 버튼 눌릴 시 -1 반환
  // 오른쪽 버튼 눌릴 시 1 반환
  // 버그?: 좌/우 버튼 누를 시 플레이어가 좌우로 '깜짝이동'을 함
  //해결. ㅇㅋ

  // 방법: 버튼 누르고 있을 때만 이동하거나 누르자마자 이동하게 수정
  int getPlayerMove() {

    unsigned long long movetime = millis();
    int left = digitalRead(PIN_BUTTON_LEFT);
    int right = digitalRead(PIN_BUTTON_RIGHT);
    if ((left == HIGH && !lmove) || (left == HIGH && movetime + 100 <= millis() && !lmove)) // 100ms 이상 누르면 이동 하거나 누르자마자 이동
    {
      lmove = true;
      return -1;
    }
    if ((right == HIGH && !rmove) || (right == HIGH && movetime + 100 <= millis() && !rmove)) // ''
    {
      rmove = true;
      return 1;
    }
    if(left == LOW){ // 버튼 떼면 움직임 멈춤
      lmove = false;
    }
    if(right == LOW){ //''
      rmove = false;
    }
    return 0;
  }
}

#endif