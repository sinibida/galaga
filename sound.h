#ifndef SOUND_H
#define SOUND_H

// 효과음 상태
#define SOUND_PLAYER_ATTACK 0
#define SOUND_GAMEOVER 1
#define SOUND_CLEAR 2
#define SOUND_DEATH 3

// BGM 상태
#define BGM_OFF 0
#define BGM_NORMAL 1
#define BGM_BOSS 2

#include <Arduino.h>

// 주나: 핀 번호 충돌 방지를 위해 다른 헤더로 모았어용 (보면 지우셈)
#include "constants.h"

namespace sound
{
  short frequencies[14] = {
      // 도, 레, 미, 파, 솔, 라, 시
      261, 293, 329, 349, 392, 440, 466,
      522, 586, 658, 698, 884, 880, 932};
  short note[6] = {
      // 사분음표, 점사분음표, 팔분음표, 점팔분음표, 십육분음표,점십육분음표
      500, 750, 250, 375, 125, 187};

  unsigned char melody[65][2]{
      // 멜로디 배열
      {0, 0},
      {3, 2},
      {0, 2},
      {3, 2},
      {4, 2},
      {5, 0},
      {4, 2},
      {5, 4},
      {6, 4},
      {7, 2},
      {5, 2},
      {6, 4},
      {0, 4},
      {3, 4},
      {5, 4},
      {4, 1},
      {3, 2},
      {2, 2},
      {3, 2},
      {0, 2},
      {3, 2},
      {0, 1},
      {3, 2},
      {4, 4},
      {5, 4},
      {4, 2},
      {5, 2},
      {6, 4},
      {4, 4},
      {5, 4},
      {6, 4},
      {0, 1},
      {3, 2},
      {9, 2},
      {10, 4},
      {9, 2},
      {6, 2},
      {5, 3},
      {5, 4},
      {6, 2},
      {0, 2},
      {3, 2},
      {3, 4},
      {4, 4},
      {5, 2},
      {6, 2},
      {5, 1},
      {4, 2},
      {0, 4},
      {0, 4},
      {3, 2},
      {0, 2},
      {3, 4},
      {4, 4},
      {5, 4},
      {4, 4},
      {5, 1},
      {6, 2},
      {11, 2},
      {3, 2},
      {9, 2},
      {4, 2},
      {3, 0},
      {2, 2},
      {3, 4},
  };
  void init()
  { // 사운드 핀 설정
    pinMode(PIN_SOUND_BUZZER_1, OUTPUT);
    pinMode(PIN_SOUND_BUZZER_2, OUTPUT);
  }

  int currentBgm = BGM_OFF;
  unsigned long long a = 0;
  unsigned long long b = 0;
  int count = 0;
  bool playi = false;

  int effectIndex;
  int effectLeft;
  int effectMillis;

  // buzzerpin, millis()
  void bgmsetup()
  { // 배경음악 시작
    count = 0;
    a = millis();
    b = millis() + note[melody[count][1]];
    if (melody[count][0] > 0)
    {
      tone(PIN_SOUND_BUZZER_1, frequencies[melody[count][0]]);
    }
    playi = true;
  }

  void setBgm(int bgm) {
    currentBgm = bgm;
  }

  // buzzerpin, millis()
  void bgmLoop()
  { // 일반 배경음악 시작
    if (currentBgm == BGM_OFF) return;

    unsigned long currenttime = millis();
    if (playi == false)
      return;
    if (count == 64)
    {
      count = 0;
    }
    if (currenttime >= b)
    {
      noTone(PIN_SOUND_BUZZER_1);
      count++;
      if (melody[count][0] > 0)
      {
        if (currentBgm == BGM_BOSS) {
          a = currenttime;
          b = millis() + (int)(note[melody[count][1]] * 1.5);
          if (effectLeft == 0)
            tone(PIN_SOUND_BUZZER_1, int(frequencies[melody[count][0]] * 1.2));
        } else if (currentBgm == BGM_NORMAL) {
          a = currenttime;
          b = millis() + note[melody[count][1]];
          if (effectLeft == 0)
            tone(PIN_SOUND_BUZZER_1, frequencies[melody[count][0]] * 2);
        }
      }
    }
  }

  // void boss_bgm_start()
  // { // 보스 배경음악 시작
  //   unsigned long currenttime = millis();
  //   if (playi == false)
  //     return;
  //   if (count == 64)
  //   {
  //     count = 0;
  //   }
  //   if (currenttime >= b)
  //   {
  //     noTone(PIN_SOUND_BUZZER_1);
  //     count++;
  //     if (melody[count][0] > 0)
  //     {
  //       a = currenttime;
  //       b = millis() + note[melody[count][1]];
  //       if (effectLeft <= 0)
  //         tone(PIN_SOUND_BUZZER_1, frequencies[melody[count][0]]/2);
  //     }
  //   }
  // }

  // buzzerpin
  void bgmstop()
  { // 배경음악 정지
    noTone(PIN_SOUND_BUZZER_1);
    playi = false;
  }

  void attacksound()
  { // 총알 소리

    tone(PIN_SOUND_BUZZER_2, effectLeft * 5 + 100);
  }

  void deathsound()
  { // 죽음 소리
    if (effectLeft / 100 % 2 == 0)
      tone(PIN_SOUND_BUZZER_2, 150);
    else
      noTone(PIN_SOUND_BUZZER_2);
  }

  void gameoversound()
  { // 죽음 소리
    tone(PIN_SOUND_BUZZER_2, effectLeft * 2 + 100);
  }

  void clearsound()
  { // 클리어 소리
    tone(PIN_SOUND_BUZZER_2, (1000 - effectLeft) * 2 + 100);
  }

  void playEffectSound(int index)
  { // 효과음 재생
    noTone(PIN_SOUND_BUZZER_2);
    effectMillis = millis();
    switch (index)
    {
    case SOUND_PLAYER_ATTACK:
      // 플레이어 총알 소리 재생 코드
      effectIndex = SOUND_PLAYER_ATTACK;
      effectLeft = 200;
      break;
    case SOUND_DEATH:
      // 죽음 소리 재생 코드
      effectIndex = SOUND_DEATH;
      effectLeft = 500;
      break;
    case SOUND_GAMEOVER:
      // 죽음 소리 재생 코드
      effectIndex = SOUND_GAMEOVER;
      effectLeft = 1000;
      break;
    case SOUND_CLEAR:
      // 죽음 소리 재생 코드
      effectIndex = SOUND_CLEAR;
      effectLeft = 1000;
      break;
    }
  }

  void effectLoop() {
    if (effectLeft > 0) {
      switch (effectIndex)
      {
      case SOUND_PLAYER_ATTACK:
        attacksound();
        break;
      case SOUND_DEATH:
        deathsound();
        break;
      case SOUND_GAMEOVER:
        gameoversound();
        break;
      case SOUND_CLEAR:
        clearsound();
        break;
      }

      unsigned long currentMillis = millis();
      effectLeft -= currentMillis - effectMillis;
      effectMillis = currentMillis;
    } else {
      noTone(PIN_SOUND_BUZZER_2);
      effectLeft = 0;
    }
  }

  // 사운드 동작을 위한 반복 함수
  // (이펙트 & BGM)
  void soundLoop() {
    effectLoop();
    bgmLoop();
  }
}

#endif