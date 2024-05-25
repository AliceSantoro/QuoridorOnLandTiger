#include "lpc17xx.h" 
#ifndef __GAME_H
#define __GAME_H

struct Player {
    uint8_t posX;
    uint8_t posY;
    uint8_t walls;
	  uint16_t color;
};


struct Wall {
    uint8_t n : 1;
    uint8_t s : 1;
    uint8_t e : 1;
    uint8_t w : 1;
};


struct CurrentWall{
   uint8_t x;
	 uint8_t y;
};

extern void startGame(void);
extern void move(struct Player p1, struct Player p2, struct Wall (*walls)[6],uint16_t color);
int moveUp(struct Player p1, struct Player p2, struct Wall (*walls)[6],uint16_t color);
int moveDown(struct Player p1,struct Player p2, struct Wall (*walls)[6],uint16_t color);
int moveLeft(struct Player p1,struct Player p2, struct Wall (*walls)[6],uint16_t color);
int moveRight(struct Player p1,struct Player p2, struct Wall (*walls)[6],uint16_t color);

int movUp(struct Player p1, struct Player p2, struct Wall (*walls)[6]);
int movDown(struct Player p1,struct Player p2, struct Wall (*walls)[6]);
int movLeft(struct Player p1,struct Player p2, struct Wall (*walls)[6]);
int movRight(struct Player p1,struct Player p2, struct Wall (*walls)[6]);

int moDown(uint8_t x, uint8_t y, struct Wall tempwalls[6][6]);
int moUp(uint8_t x, uint8_t y, struct Wall tempwalls[6][6]);
int moLeft(uint8_t x, uint8_t y, struct Wall tempwalls[6][6]);
int moRight(uint8_t x, uint8_t y, struct Wall tempwalls[6][6]);

int checkVictory(struct Player p1, struct Player p2);

void changeUp(struct Player *p);
void changeDown(struct Player *p);
void changeLeft(struct Player *p);
void changeRight(struct Player *p);

void ChangeMode(void);
void changeWallDirection(void);

int placeWall(uint8_t x, uint8_t y, struct Player p, struct Wall (*walls)[6]);
void finalPlaceWall( uint8_t x, uint8_t y,struct Player *p, struct Wall (*walls)[6]);
void controlPrevious(uint8_t x, uint8_t y,struct Wall (*walls)[6]);	

void saveValues(volatile uint32_t *variable);
void saveOutOfTime(volatile uint32_t *variable);

int checkTrap(void);
int recursiveSolve(struct Player p, uint8_t x, uint8_t y,uint8_t (*wasH)[7], struct Wall (tempWalls[6][6]));

#endif /* end __GAME_H */
