#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  unsigned long interval;
  unsigned long lastT;
} Timer;

typedef struct {
  unsigned char x;
  unsigned char y;
  short health;
  unsigned char texture;
  Timer movement;
} Enemy;

typedef struct {
  unsigned char x;
  unsigned char y;
  unsigned char direction;
  short damage;
  unsigned char texture;
  bool hitPlayer;
  Timer movement;
} Bullet;

#endif
