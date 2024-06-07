#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

#include "constants.h"
#include "graphic.h"  //에러 아님
#include "io.h"
#include "score.h"
#include "sound.h"
#include "structs.h"

#define MAX_ENEMY_COUNT 32  // 최대 적 수
Enemy enemies[MAX_ENEMY_COUNT] = {};
size_t enemiesCnt = 0;  // For test: 0이 기본값

Bullet bullets[MAX_ENEMY_COUNT] = {};  // 탄환 스택
size_t bulletsCnt = 0;

Timer enemySpawn = {1250, 0};  // 적 생성 주기
Timer redraw = {100, 0};    // 화면 갱신 간격

int playerPosition = 1;  // 플레이어 위치
int shootPos = 0;     // 발사 위치
int life = 3;       // 생명
int score = 0;       // 점수
int stage = 0;       // 스테이지 번호
int stageProgress = 0;   // 다음 스테이지까지 소환할 적 수
int currentBossIdx = -1;  // 현재 보스 인덱스 (enemies[currentBossIdx] = 보스)
int bossKilledCnt = 0;   // 보스 처치 수 (1 == 클리어)
int patternOld = 0;    // 패턴 랜덤 변수가 너무 중복되는걸 방지하기 위한 변수
unsigned long long gameoverT = 0;  // 게임오버 대기 종료될 시간
int gameoverState = 0;

// 게임 초기 상태 설정
void resetGame() {
  life = 3;
  enemiesCnt = 0;
  bulletsCnt = 0;
  enemySpawn.lastT = millis();
  playerPosition = 1;
  stage = 0;
  stageProgress = 20; // 보스 이전 난이도 조절 시 변경
  currentBossIdx = -1;
  bossKilledCnt = 0;
  enemySpawn.interval = 1600;
  gameoverT = 0;
  scoreboard::resetScore();
  graphic::reset();
  sound::bgmsetup();
  sound::setBgm(BGM_NORMAL);
}

// 게임오버 시 실행
void gameOver() {
  sound::setBgm(BGM_OFF);
  sound::bgmstop();
  sound::playEffectSound(SOUND_GAMEOVER);
  io::lifeViewer(0);
  scoreboard::scoreSort();
}

// 게임 클리어 시 실행
void clear() {
  sound::setBgm(BGM_OFF);
  sound::bgmstop();
  sound::playEffectSound(SOUND_CLEAR);
  io::lifeViewer(3);
  scoreboard::scoreSort();
}

// 플레이어 탄환 생성
void createPlayerBullet() {
  if (shootPos == 0) {
    shootPos = 1;
  } else if (shootPos == 1) {
    shootPos = 0;
  }

  int yPos = playerPosition + shootPos;
  if (yPos < 0 || yPos >= 4) {
    return;
  }

  Bullet& at = bullets[bulletsCnt];
  at.x = 1;
  at.y = playerPosition + shootPos;
  at.direction = 1;
  at.damage = 1;
  at.texture = BULLET_PLAYER;
  at.hitPlayer = 0;
  at.movement.interval = 150;
  at.movement.lastT = millis();

  bulletsCnt++;
}

// 탄환 제거
void removeBullet(int idx) {
  bulletsCnt--;

  for (int i = idx; i < bulletsCnt; i++) {
    bullets[i] = bullets[i + 1];
  }
}

// 입력된 y 좌표에 적 생성
void createEnemy1(int y) {
  Enemy& at = enemies[enemiesCnt];
  Enemy temp = {19, y, 4, 0};
  temp.movement.interval = 500;
  temp.movement.lastT = millis();
  at = temp;

  enemiesCnt++;
}

// 입력된 y 좌표에 적 생성
void createEnemy2(int y) {
  Enemy& at = enemies[enemiesCnt];
  Enemy temp = {19, y, 3, 1};
  temp.movement.interval = 2500;
  temp.movement.lastT = millis();
  at = temp;

  enemiesCnt++;
}

// 입력된 좌표에 보스 생성
void createEnemyBoss(int x, int y) {
  Enemy& at = enemies[enemiesCnt];
  Enemy temp = {x, y, 20, 10};  // x y health texture
  temp.movement.interval = 5000;
  temp.movement.lastT = millis();
  at = temp;

  currentBossIdx = enemiesCnt;
  enemiesCnt++;
}

// 보스가 소환하는 잡몹 n은 소환할 일반몹의 수
void createBossSummoned(int n) {
  int summonedY[4] = {0, };
  for (int i = 0; i < n; i++) {
    int y = random(4);
    while (summonedY[y] == 1) {
      y = random(4);
    }
    summonedY[y] = 1;

    Enemy& at = enemies[enemiesCnt];
    Enemy temp = {19, y, 6-n, 2}; // 5, 4, 3
    temp.movement.interval = 300;
    temp.movement.lastT = millis() + random(300);
    at = temp;
    enemiesCnt++;
  }
}

// 플레이어를 향하는 총알 생성
void createEnemyBullet(int x, int y, int texture = BULLET_ENEMY) {
  Bullet& at = bullets[bulletsCnt];
  at.x = x;
  at.y = y;
  at.direction = -1;
  at.damage = 1;
  at.texture = texture;
  at.hitPlayer = 1;
  at.movement.interval = 150;
  at.movement.lastT = millis();
  bulletsCnt++;
}

// 보스가 소환하는 탄환 패턴
int bossBulletPatterns[3][3] = {
  {0, -100, -100},  // 중앙
  {1, -1, -100},   // 좌우
  {1, -1, 0}     // 트리플샷
};
// 보스가 탄환의 수 n은 소환할 탄환의 수
void createBossBullet(int n) {
  int bossPos = enemies[currentBossIdx].y;
  for (int i = 0; i < n; i++) {
    createEnemyBullet(18, bossPos + bossBulletPatterns[n - 1][i], BULLET_BOSS);
  }
}


void removeEnemy(int idx) {  // 적(잡몹) 제거
  enemiesCnt--;
  for (int i = idx; i < enemiesCnt; i++) {
    enemies[i] = enemies[i + 1];
  }
  if (idx < currentBossIdx) {
    currentBossIdx--;
  }
}

// 이동 및 발사 처리
void handleInput() {
  int move = io::getPlayerMove();
  playerPosition += move;
  if (playerPosition < -1) {
    playerPosition = -1;
  } else if (playerPosition > 3) {
    playerPosition = 3;
  }

  int shoot = io::getShoot(); 
  if (shoot > 0) {
    createPlayerBullet();
    if (shoot != 2) {
      sound::playEffectSound(SOUND_PLAYER_ATTACK);
    }
  }
}

int handleBulletMovement(Bullet& b, int i) {  // 총알 이동 처리
  b.x += b.direction;
  if (b.x > 19) {
    removeBullet(i);
    i--;
    return -1;
  }

  return 0;
}

int handleEnemyMovement(Enemy& e, int i) {  // 적 이동 처리
  if (i == currentBossIdx) {
    if (e.y < 2) {
      e.y++;
    } else {
      e.y--;
    }
    return 0;
  } else {
    e.x--;
    if (e.texture == 1) {
      createEnemyBullet(e.x, e.y);
    }

    if (e.x <= 0) {
      removeEnemy(i);
      i--;
      life--;
      return -1;
    }

    return 0;
  }
}

// 총알이 플레이어/적이랑 충돌할 때
void handleBulletCollision() {
  for (int i = 0; i < bulletsCnt; i++) {
    Bullet& b = bullets[i];

    if (b.hitPlayer == 0) {
      // 적 & 총알 충돌 처리
      for (int j = 0; j < enemiesCnt; j++) {
        Enemy& e = enemies[j];
        if (e.x == b.x && e.y == b.y) {
          e.health -= b.damage;
          if (e.health <= 0) {
            int isBoss = j == currentBossIdx;
            if (isBoss) {
              currentBossIdx = -1;
              bossKilledCnt++;
            }

            removeEnemy(j);
            scoreboard::getScore(isBoss);

            stageProgress--;
            j--;
          }
          removeBullet(i);
          i--;
          break;
        }
      }
    } else {
      // 플레이어 & 총알 충돌 처리
      if (b.x == 1 && (b.y == playerPosition || b.y == playerPosition + 1)) {
        life--;
        removeBullet(i);
        i--;
      }
    }
  }
}

void clearEnemyBullets() {
  for (int i = 0; i < bulletsCnt; i++) {
    if (bullets[i].texture == BULLET_ENEMY) {
      removeBullet(i);
      i--;
    }
  }
}

void handleEnemySpawn() {
  switch (stage)  // 이렇게 하는게 맞나?
    {
      case 0: {
        int pattern = random(5 - patternOld);
        if (pattern > 0)  // 패턴 1
        {
          createEnemy1(random(4));
          patternOld++;
        } else if (pattern == 0)  // 패턴 2
        {
          createEnemy2(random(4));
          patternOld = 0;
        }

        if (stageProgress <= 0) {
          stage++;
          stageProgress = 2000;
          enemiesCnt = 0;
          clearEnemyBullets();
          createEnemyBoss(18, 1);
          enemySpawn.interval = 3000;
          enemySpawn.lastT = millis() + 1000;
          graphic::setTextureSet(SET_BOSS);
          sound::setBgm(BGM_BOSS);
          patternOld = 0;
        }
      } break;
      case 1: {
        int pattern = random(2);
        if (pattern == 0)  // 패턴 1 이거 어떻게 하지 적 소환 패턴
        {
          createBossSummoned(random(1, 4));
        } else if (pattern == 1)  // 패턴 2 이거 어떻게 하지 총알 소환 패턴
        {
          createBossBullet(random(1, 4));
        }
      } break;
    }
}

// LCD에 그리기
void drawAndView() {
  graphic::clear();
  graphic::drawBullets(bullets, bulletsCnt);
  graphic::drawEnemies(enemies, enemiesCnt);
  graphic::drawPlayer(playerPosition);

  io::lifeViewer(life);
}

int gameLoop() {
  // 게임오버 대기중
  if (gameoverT > 0) {
    if (millis() >= redraw.lastT + redraw.interval) {
      drawAndView();

      redraw.lastT = millis();
    }

    if (millis() >= gameoverT) {
      switch (gameoverState)
      {
      case STATE_GAME_OVER:
        gameOver();
        return gameoverState;
      case STATE_CLEAR:
        clear();
        return gameoverState;
      }
    }
    return STATE_GAME;
  }
  
  handleInput();

  // 각 총알마다 이동 처리
  for (int i = 0; i < bulletsCnt; i++) {
    Bullet& b = bullets[i];
    if (millis() >= b.movement.lastT + b.movement.interval) {
      if (handleBulletMovement(b, i) == 0)
        b.movement.lastT = millis();
    }
  }

  // 각 적마다 이동 처리
  for (int i = 0; i < enemiesCnt; i++) {
    Enemy& e = enemies[i];
    if (millis() >= e.movement.lastT + e.movement.interval) {
      if (handleEnemyMovement(e, i) == 0)
        e.movement.lastT = millis();
    }
  }

  // 총알 충돌 처리
  handleBulletCollision();

  if (millis() >= enemySpawn.lastT + enemySpawn.interval) {
    handleEnemySpawn();

    enemySpawn.lastT = millis();
  }

  if (millis() >= redraw.lastT + redraw.interval) {
    drawAndView();

    redraw.lastT = millis();
  }
  
  // 게임오버: 생명 <= 0
  if (life <= 0) {
    gameoverT = millis() + 1500;
    enemiesCnt = 0;
    bulletsCnt = 0;
    sound::setBgm(BGM_OFF);
    sound::bgmstop();
    sound::playEffectSound(SOUND_DEATH);
    gameoverState = STATE_GAME_OVER;
  }

  // 클리어: 보스 처치 수 >= 1
  if (bossKilledCnt >= 1) {
    gameoverT = millis() + 2000;
    enemiesCnt = 0;
    bulletsCnt = 0;
    sound::setBgm(BGM_OFF);
    sound::bgmstop();
    sound::playEffectSound(SOUND_DEATH);
    gameoverState = STATE_CLEAR;
  }

  return STATE_GAME;
}
#endif