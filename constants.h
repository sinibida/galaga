//자리 비울 때 항상 여기에 있음. 버그 있으면 텔포 시켜주세요.
#ifndef CONSTANTS_H
#define CONSTANTS_H 

#define PIN_SOUND_BUZZER_1 3 // 사운드
#define PIN_SOUND_BUZZER_2 4 // 사운드

#define PIN_LED_1 6 // LIFE LED
#define PIN_LED_2 7 // LIFE LED
#define PIN_LED_3 8 // LIFE LED

#define PIN_BUTTON_SHOOT 9 // 발사 버튼
#define PIN_BUTTON_LEFT 10 // 좌측 버튼
#define PIN_BUTTON_RIGHT 11 // 우측 버튼

#define PIN_LCD_SDA A4 // I2C
#define PIN_LCD_SCL A5 // I2C

#define STATE_START 1 // 게임 시작
#define STATE_GAME 2 // 게임 중
#define STATE_GAME_OVER 3 // 게임 오버
#define STATE_CLEAR 4 // 게임 클리어
#define STATE_SCORE 11 // 점수 표시

#define BULLET_PLAYER 0
#define BULLET_ENEMY 1
#define BULLET_BOSS 2

#endif
//중복 include 방지