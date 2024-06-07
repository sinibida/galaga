#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "structs.h"
#include "dot.h"
#include "score.h"
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

#include "constants.h"

// +) lcd 함수 목록 보기
// => https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.h

#define SET_NORMAL 1
#define SET_BOSS 2

namespace graphic
{                                                     // 그래픽 관련 함수들
  LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT); // 적이 char_03까지 포함된거죠?네 적이 char03이고
  // 쟤들을 00 01 02 03 좌표에 배치하고 쟤들로 공격을 하려구요 그럼 맞아요 ㅇㅋ 보스는 잘 조립 된 것으로
  //                    0플레이어-상 1플레이어-하  2탄환  3강화 적1 4보스  하  5보스 좌    6보스 우  7 보스 상
  byte *bossStage[8] = {char_00,    char_01,  char_02,  char_09,  char_05,  char_06, char_07, char_08};
  //                    0플레이어-상 1플레이어-하   2탄환    3탄환2    4적1      5적2     6강화 적 2
  byte *normalStage[8] = {char_00, char_01, char_02, char_12, char_03, char_04, char_10};
  
  // 텍스쳐 세트 설정
  void setTextureSet(int set) {
    switch (set)
    {
    case SET_NORMAL:
      for (int i = 0; i < 8; i++)
      {
        lcd.createChar(i, normalStage[i]);
      }
      break;
    case SET_BOSS:
      for (int i = 0; i < 8; i++)
      {
        lcd.createChar(i, bossStage[i]);
      }
      break;
    }
  }

  void clear()
  {
    lcd.clear();
  }

  // 플레이어를 그림
  // 주의: 2칸 짜리임
  void drawPlayer(int at)
  {
    if (0 <= at && at < LCD_HEIGHT)
    { // !!!: 4줄짜리 LCD 사용 시 수정
      lcd.setCursor(0, at);
      lcd.print("\u0008");
    }
    int at1 = at + 1;
    if (0 <= at1 && at1 < LCD_HEIGHT)
    { // !!!: 4줄짜리 LCD 사용 시 수정
      lcd.setCursor(0, at1);
      lcd.print("\u0001");
    }
  }

  void drawScoreInGame(int score) // 구현 안 할 가능성 높음(게임 플레이에 영향 미침)
  {
    // TODO
    lcd.setCursor(10, 0);
    lcd.print(score);
  }

  // 보스 그리기(자세한 내용은 수정 필요) bossinit()함수를 통해 쓰일 시점에 미리 만들어야 함. + 보스는
  void drawBoss(int x, int y)
  { // 4보스-하    5보스-좌    6보스-우  7보스-상
    lcd.setCursor(x, y); // 보스 상 (머리)
    lcd.print("\u0004");     // 사용자 정의 문자 7 (보스상)
    lcd.setCursor(x, y+1);     // 보스 하 (꼬리)
    lcd.print("\u0005");     // 사용자 정의 문자 4 (보스하)
    lcd.setCursor(x - 1, y); // 보스 좌측 (왼쪽 팔)
    lcd.print("\u0006");     // 사용자 정의 문자 5 (보스 좌)
    lcd.setCursor(x + 1, y); // 보스 우측 (오른쪽 팔)
    lcd.print("\u0007");     // 사용자 정의 문자 6 (보스 우)
  }

  void drawEnemies(Enemy data[], int cnt)
  { // 적 그리기
    for (int i = 0; i < cnt; i++)
    {
      Enemy enemy = data[i];           // 적의 주소를 가져옴
      switch (enemy.texture)
      {
      case 0: // 일반 적
        lcd.setCursor(enemy.x, enemy.y); // 적 X Y 좌표로 이동(LCD에 커서 업데이트)
        lcd.print("\u0004");             // 적 그리기
        break;
      case 1: // 저격충
        lcd.setCursor(enemy.x, enemy.y); // 적 X Y 좌표로 이동(LCD에 커서 업데이트)
        lcd.print("\u0005");
        break;             // 적 그리기
      case 2: // 소환수
        lcd.setCursor(enemy.x, enemy.y); // 적 X Y 좌표로 이동(LCD에 커서 업데이트)
        lcd.print("\u0003");             // 적 그리기
        break;
      case 10:
        drawBoss(enemy.x, enemy.y);
        break;
      default:
        break;
      }
    }
  }

  void drawBullets(Bullet data[], int cnt)
  { // 탄환 그리기
    for (int i = 0; i < cnt; i++)
    {
      Bullet &bullet = data[i];          // 탄환의 주소를 가져옴
      lcd.setCursor(bullet.x, bullet.y); // 탄환 X Y 좌표로 이동(LCD에 커서 업데이트)
      switch (bullet.texture)
      {
      case BULLET_PLAYER:
        lcd.print("\u0002"); // 탄환 그리기
        break;
      case BULLET_ENEMY:
        lcd.print("\u0003"); // 탄환 그리기
        break;
      case BULLET_BOSS:
        lcd.print("<+"); // 탄환 그리기
        break;
      }
    }
  }

  // 시작화면 그리기
  void drawStartMenu()
  {
    lcd.setCursor(3, 0);
    lcd.print("Deep Blue Sea");
    lcd.setCursor(3, 2);
    lcd.print("START->SHOOT");
    lcd.setCursor(3, 3);
    lcd.print("RANKING->RIGHT");
  }
  // 스코어보드 그리기
  void drawScoreBoard()
  {
    lcd.setCursor(0, 0);
    lcd.print("BEST");
    lcd.setCursor(0, 1);
    lcd.print("1. ");
    lcd.print(scoreboard::scoreRecord[0]);
    lcd.setCursor(0, 2);
    lcd.print("2. ");
    lcd.print(scoreboard::scoreRecord[1]);
    lcd.setCursor(0, 3);
    lcd.print("3. ");
    lcd.print(scoreboard::scoreRecord[2]);
  }

  // 클리어화면 그리기
  void drawClear()
  {
    lcd.setCursor(10, 0);
    lcd.print("SCORE:");
    lcd.print(scoreboard::finalScore);
    lcd.setCursor(8, 1);
    lcd.print("CLEAR!!");
    lcd.setCursor(10, 2);
    lcd.print("RESTART");
    lcd.setCursor(7, 3);
    lcd.print("Press->SHOOT");

    drawScoreBoard();
  }
  // 게임오버화면 그리기
  void drawGameOver()
  {
    lcd.setCursor(10, 0);
    lcd.print("SCORE:");
    lcd.print(scoreboard::finalScore);
    lcd.setCursor(8, 1);
    lcd.print("GAMEOVER");
    lcd.setCursor(10, 2);
    lcd.print("RESTART");
    lcd.setCursor(7, 3);
    lcd.print("Press->SHOOT");

    drawScoreBoard();
  }

  // 보스 초기화 여기서부터
  void bossinit()
  {
    for (int i = 0; i < 8; i++)
    {
      lcd.createChar(i, bossStage[i]);
    }
  }

  // 초기화 (lcd.clear & 텍스쳐 세트 설정 & etc...)
  void reset(){
    clear();
    setTextureSet(SET_NORMAL);
  }

  void init()
  { // lcd초기화 및 노말 스테이지 적 생성 준비
    lcd.init();
    lcd.backlight();
    reset();
  }
}

#endif