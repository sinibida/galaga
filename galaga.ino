#include "graphic.h" //에러 아님
#include "sound.h"
#include "io.h"
#include "structs.h"
#include "constants.h"
#include "game.h"

void setup() {
  graphic::init();
  sound::init();
  io::init();

  randomSeed(analogRead(0));

  // 디버그용
  // Serial.begin(9600);
}

int state = STATE_START;

void restartGame() {
  state = STATE_GAME;
  
  resetGame();
}

#define DT 33

void loop() {
  sound::soundLoop();
  switch(state) { // state 0, 1, 2, 3 값에 따라 상황 전환
    case STATE_START: // 시작화면
      {
        graphic::drawStartMenu();
        if (io::getShoot()) {
          graphic::clear();
          restartGame();
        }
        if (io::getRight())
        {
          graphic::clear();
          state = STATE_SCORE;
        }
        delay(DT);
      }
      break;
    case STATE_GAME: // 게임 중
      {
        int result = gameLoop();
        if (result != STATE_GAME) {
          graphic::clear();
          state = result;
        }
      }
      break;
    case STATE_GAME_OVER: // 게임 종료
      graphic::drawGameOver();
      if (io::getRight()) {
        graphic::clear();
        state = STATE_START;
      }
      if (io::getShoot()) {
        graphic::clear();
        restartGame();
      }
      delay(DT);
      break;
    case STATE_CLEAR: // 클리어
      graphic::drawClear();
      if (io::getRight()) {
        graphic::clear();
        state = STATE_START;
      }
      if (io::getShoot()) {
        graphic::clear();
        restartGame();
      }
      delay(DT);
      break;
    case STATE_SCORE: // 점수화면
      graphic::drawScoreBoard();
      delay(DT);
      if (io::getRight()) {
        graphic::clear();
        state = STATE_START;
      }
      break;
  }
}
