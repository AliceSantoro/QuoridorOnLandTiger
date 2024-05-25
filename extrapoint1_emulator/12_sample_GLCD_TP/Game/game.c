#include "lpc17xx.h"
#include "game.h"
#include "../GLCD/GLCD.h"  
#include "../timer/timer.h"
#include "../RIT/RIT.h"

volatile uint8_t turn = 0;

struct Player red; 
struct Player white;
struct Wall Walls[6][6];

volatile uint8_t victory =0;

extern int turn_time;
extern enum Gamemode currentMode;
extern enum WallDirection currentWallDirection;
extern struct CurrentWall currentWall;

void startGame(void){
		
	uint8_t i=0;
	uint8_t j=0;
	
	
	
	 for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6; ++j) {
            Walls[i][j].n = 0;
            Walls[i][j].s = 0;
            Walls[i][j].e = 0;
            Walls[i][j].w = 0;
        }
    }//walls matrix initialised to 0 
	
	
	red.posX = 3;
	red.posY = 0;
	red.walls = 8;
	red.color = 1; //player 1 initialised
	
	white.posX = 3;
	white.posY = 6;
	white.walls = 8;
	white.color=0; //player 2 initialised
  
	colorSquare(109,13,Red);
	colorSquare(109,205,White);
	
	printVariable(37, 285, red.walls, White, Black);
	printVariable(195, 285, white.walls, White, Black);
	

	
}

/* move hilights the possible moves of a player*/

void move(struct Player p1, struct Player p2, struct Wall (*walls)[6], uint16_t color) {
 
	moveUp(p1, p2, walls, color);
	moveDown(p1,p2,walls, color);
	moveLeft(p1,p2,walls, color);
	moveRight(p1,p2,walls, color);

}

/* moveUp checks if a player can go up and highlites the move */

int moveUp(struct Player p1, struct Player p2, struct Wall (*walls)[6], uint16_t color){
	
	struct Player nextP; 
	
	nextP.posX=p1.posX;
	nextP.posY= p1.posY - 1;
  nextP.walls=p1.walls;
	
	if(p1.posY==0) return 0;//out of borders 
	
	if(p1.posX >0) {if(walls[p1.posY -1][p1.posX-1].e==1) return 0;}
	
	if(p1.posX <6){if (walls[p1.posY-1][p1.posX].w==1) return 0;}      
	
	if(nextP.posX==p2.posX && nextP.posY==p2.posY){
		if(p1.posY-1==0)return 0;
		if(p1.posX >0){if(walls[p1.posY-2][p1.posX -1].e==1) return 0;}
		if(p1.posX <6){ if(walls[p1.posY-2][p1.posX].w==1)return 0;}
		nextP.posY--;
	}
	
	colorSquare(13+32*nextP.posX, 13+32*nextP.posY, color);
	return 1;
}

/* moveDown checks if a player can go down and highlites the move*/

int moveDown(struct Player p1,struct Player p2, struct Wall (*walls)[6],uint16_t color){

	  struct Player nextP; 
	  nextP.posX=p1.posX;
	  nextP.posY= p1.posY + 1;
    nextP.walls=p1.walls;
	
	  if(nextP.posY>6) return 0;//out of borders 
	
		if(p1.posX >0){if(walls[p1.posY][p1.posX -1].e == 1) return 0;}
		if(p1.posX<6){if(walls[p1.posY][p1.posX].w ==1) return 0;}
	  
	  if(nextP.posX==p2.posX && nextP.posY==p2.posY){
			if(p1.posY+2>6)return 0;
			if(p1.posX >0){if(walls[p1.posY+1][p1.posX -1].e==1) return 0;}
		  if(p1.posX<6){if(walls[p1.posY+1][p1.posX].w==1) return 0;} 
				nextP.posY++;
			}
		
		colorSquare(13+32*nextP.posX, 13+32*nextP.posY, color);
			
		return 1;
}

/* moveLeft checks if a player can go left and highlites the move*/

int moveLeft(struct Player p1,struct Player p2, struct Wall (*walls)[6],uint16_t color){
		
	struct Player nextP; 
	if(p1.posX==0) return 0;//out of borders 
	
	nextP.posX=p1.posX-1;
	nextP.posY= p1.posY;
  nextP.walls=p1.walls;
	
	  
	if(p1.posY>0){if(walls[p1.posY-1][p1.posX -1].s==1) return 0;}
	if(p1.posY <6){if( walls[p1.posY][p1.posX-1].n==1) return 0;}
	if(nextP.posX==p2.posX && nextP.posY==p2.posY){
			if(p1.posX-2<0)return 0;
			
		  if(p1.posY>0){if(walls[p1.posY-1][p1.posX -2].s==1) return 0;}
			if(p1.posY <6){if(walls[p1.posY][p1.posX-2].n==1) return 0;}
				nextP.posX--; //jump over
			}
	
		colorSquare(13+32*nextP.posX, 13+32*nextP.posY, color);
    return 1;
}

/* moveRight checks if a player can go right and hghlites the move */

int moveRight(struct Player p1,struct Player p2, struct Wall (*walls)[6],uint16_t color){

		struct Player nextP; 
		if(p1.posX==6) return 0;//out of borders 
	
		nextP.posX=p1.posX+1;
		nextP.posY= p1.posY;
		nextP.walls=p1.walls;
	
	  
		if(p1.posY>0){if(walls[p1.posY-1][p1.posX].s==1) return 0;}//blocco muro 
		if(p1.posY <6){if( walls[p1.posY][p1.posX].n==1) return 0;}
		if(nextP.posX==p2.posX && nextP.posY==p2.posY){
			if(p1.posX+2>6)return 0;
			if(p1.posY>0){if(walls[p1.posY-1][p1.posX +1].s==1) return 0;}
			if(p1.posY <6){if( walls[p1.posY][p1.posX+1].n==1) return 0;}
				nextP.posX++; //jump over
			}
		colorSquare(13+32*nextP.posX, 13+32*nextP.posY, color);
    return 1;



}	

/* movDown checks if a player can go down from his current position*/
int movDown(struct Player p1,struct Player p2, struct Wall (*walls)[6]){
	
    struct Player nextP; 
	  nextP.posX=p1.posX;
	  nextP.posY= p1.posY + 1;
    nextP.walls=p1.walls;
	
	  if(nextP.posY>6) return 0;//out of borders 
		if(p1.posX >0){if(walls[p1.posY][p1.posX -1].e == 1) return 0;}
		if(p1.posX<6){if(walls[p1.posY][p1.posX].w ==1) return 0;}
	  
	  if(nextP.posX==p2.posX && nextP.posY==p2.posY){
			if(p1.posY+2>6)return 0;
			
			if(p1.posX >0){if(walls[p1.posY+1][p1.posX -1].e==1) return 0;}
		  if(p1.posX<6){if(walls[p1.posY+1][p1.posX].w==1) return 0;} 
				nextP.posY++; //jump over
			}
		return 1;

}

/* moDown checks if a player can go down from a given (x,y) position */

int moDown(uint8_t x, uint8_t y, struct Wall tempWalls[6][6]){
	
	  
   
	  if(y==6) return 0;//out of borders 
		if(x >0){if(tempWalls[y][x-1].e == 1) return 0;}
		if(x<6){if(tempWalls[y][x].w ==1) return 0;}
		
		return 1;

}

/* movLeft checks if a player can go left from his current position */

int movLeft(struct Player p1,struct Player p2, struct Wall (*walls)[6]){struct Player nextP; 
	if(p1.posX==0) return 0;//out of borders 
	
	nextP.posX=p1.posX-1;
	nextP.posY= p1.posY;
  nextP.walls=p1.walls;
	
	  
	if(p1.posY>0){if(walls[p1.posY-1][p1.posX -1].s==1) return 0;}//blocco muro 
	if(p1.posY <6){if( walls[p1.posY][p1.posX-1].n==1) return 0;}
	if(nextP.posX==p2.posX && nextP.posY==p2.posY){
			if(p1.posX-2<0)return 0;
			
		  if(p1.posY>0){if(walls[p1.posY-1][p1.posX -2].s==1) return 0;}
			if(p1.posY <6){if(walls[p1.posY][p1.posX-2].n==1) return 0;}
				nextP.posX--; //jump over
			}
    return 1;}


/* moLeft checks if a player can go left from a given (x,y) position */
		
int moLeft(uint8_t x, uint8_t y, struct Wall tempWalls[6][6]){
	
if(x==0) return 0;//out of borders 
	
	if(y>0){if(tempWalls[y-1][x -1].s==1) return 0;}//blocco muro 
	if(y <6){if( tempWalls[y][x-1].n==1) return 0;}
	return 1;

}

/* movRight checks if a player can go right from a given (x,y) position */

int movRight(struct Player p1,struct Player p2, struct Wall (*walls)[6]){
struct Player nextP; 
		if(p1.posX==6) return 0;//out of borders 
	
		nextP.posX=p1.posX+1;
		nextP.posY= p1.posY;
		nextP.walls=p1.walls;
	
	  
		if(p1.posY>0){if(walls[p1.posY-1][p1.posX].s==1) return 0;}
		if(p1.posY <6){if( walls[p1.posY][p1.posX].n==1) return 0;}
		if(nextP.posX==p2.posX && nextP.posY==p2.posY){
			if(p1.posX+2>6)return 0;
			if(p1.posY>0){if(walls[p1.posY-1][p1.posX +1].s==1) return 0;}
			if(p1.posY <6){if( walls[p1.posY][p1.posX+1].n==1) return 0;}
				nextP.posX--; //jump over
			}
	
    return 1;}

/* moRight checks if a player can go right from a given (x,y) position */

int moRight(uint8_t x, uint8_t y, struct Wall tempWalls[6][6]){

    if(x==6) return 0;//out of borders 
	
		if(y>0){if(tempWalls[y-1][x].s==1) return 0;}
		if(y <6){if( tempWalls[y][x].n==1) return 0;}
		
    return 1;

}

/* movUp checks if a player can go up from his current position */

int movUp(struct Player p1, struct Player p2, struct Wall (*walls)[6]){
	
	struct Player nextP; 
	if(p1.posY==0) return 0;//out of borders 
	
	nextP.posX=p1.posX;
	nextP.posY= p1.posY - 1;
  nextP.walls=p1.walls;
	
	if(p1.posX >0) {if(walls[p1.posY-1][p1.posX -1].e==1) return 0;}
	if(p1.posX <6){if (walls[p1.posY-1][p1.posX].w==1) return 0;}     
	if(nextP.posX==p2.posX && nextP.posY==p2.posY){
		if(p1.posY-1==0)return 0;
		if(p1.posX >0){if(walls[p1.posY-2][p1.posX -1].e==1) return 0;}
		if(p1.posX <6){ if(walls[p1.posY-2][p1.posX].w==1)return 0;}
		nextP.posY--; //jump over
	}
	return 1;}

/* moUp checks if a player can go up from a given (x,y) position */

int moUp(uint8_t x, uint8_t y, struct Wall tempWalls[6][6]){

   if(y==0) return 0;//out of borders 

	if(x >0){if(tempWalls[y-1][x -1].e==1) return 0;}
	if(x <6){if (tempWalls[y-1][x].w==1) return 0;}     
	
	return 1;

}

/*checkVictory checks if a player has reached the opposite side*/

int checkVictory(struct Player p1, struct Player p2){
	
	if(p1.posY==0||p2.posY==6) return 1; 
	return 0;
	
}

/*changeUp moves a token up and updates its position*/

void changeUp(struct Player *p) {
	
    colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Black);
	
    if (turn == 0) {
        move(white, red, Walls, Black);
    } else {
        move(red, white, Walls, Black);
    }
		
    p->posY--; //update position 
		
		if(turn==0){ //jump over
			if(p->posY==red.posY && p->posX==red.posX){ 
				p->posY--;
			}
		}
    else{
			if(p->posY==white.posY && p->posX==white.posX){ 
				p->posY--; 
		}}
		
    if (turn == 1) {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Red);
    } else {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, White);
    }
		
		if(checkVictory(white,red)){
			victory ++; 
		}
}

/*changeDown moves a token down and updates its position*/

void changeDown(struct Player *p) {
	
    colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Black);
	
    if (turn == 1) {
        move(white, red, Walls, Black);
    } else {
        move(red, white, Walls, Black);
    }
		
		p->posY++; //update position 
		
		if(turn==0){ //jump over
			if(p->posY==white.posY&&p->posX==white.posX){
				p->posY++;
			}
		}
    else{
			if(p->posY==red.posY&&p->posX==red.posX){ 
				p->posY++; 
			}
		}
    
		
    if (turn == 1) {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, White);
    } else {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Red);
    }
		
		if(checkVictory(white,red)){victory ++; }
		
}

/*changeLeft moves a token left and updates its position*/

void changeLeft(struct Player *p) {
	
    colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Black);
	
    if (turn == 0) {
        move(white, red, Walls, Black);
    } else {
        move(red, white, Walls, Black);
    }
		
    p->posX--;//update position 
		
    if(turn==1){//jump over
				if(p->posX==white.posX&&p->posY==white.posY){ 
			p->posX--; }
		}
    else{
			if(p->posX==red.posX&&p->posY==red.posY){
				p->posX--;
			}
		}
		
    if (turn == 0) {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, White);
    } else {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Red);
    }
		if(checkVictory(white,red)){victory ++;}
}

/*changeRight moves a token right and updates its position*/

void changeRight(struct Player *p) {
	
    colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Black);
	
    if (turn == 0) {
        move(white, red, Walls, Black);
    } else {
        move(red, white, Walls, Black);
    }
		
    p->posX++;//update position 
		
		if(turn==0){ //jump over
			if(p->posX==red.posX&&p->posY==red.posY){ 
				p->posX++;
			}
		}
    else{
			if(p->posX==white.posX&&p->posY==white.posY){ 
				p->posX++;
			}
		}
		
    if (turn == 0) {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, White);
    } else {
        colorSquare(13 + 32 * p->posX, 13 + 32 * p->posY, Red);
    }
		if(checkVictory(white,red)){victory ++; LCD_Clear(Black);}
}

/*changeMode changes the game mode from PLAYER_PLACEMENT to WALL_PLACEMENT and vice versa*/

void ChangeMode(void){
	
	if(currentMode==PLAYER_PLACEMENT){currentMode=WALL_PLACEMENT;}else{currentMode=PLAYER_PLACEMENT;}

}

/*changeWallDirection changes the current wall direction from HORIZONTAL to VERTICAL and vice versa*/

void changeWallDirection(void){
	
	if(currentWallDirection == HORIZONTAL){
		currentWallDirection = VERTICAL;
	}else{
		currentWallDirection = HORIZONTAL;
	}

}

/* placeWall cheks if a wall can be placed in the current position  */

int placeWall(uint8_t x, uint8_t y, struct Player p, struct Wall (*walls)[6]){
   
	if(currentWallDirection==HORIZONTAL){
		colorHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
	}else{
		colorVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y );
	}
	
	if(p.walls==0) return 0; //no more walls
	
	if(checkTrap())return 0; //player trapped
	
	if(currentWallDirection==HORIZONTAL){
		
		if(walls[y][x].w==1 && walls[y][x].e == 1 ||  walls[y][x].n==1 && walls[y][x].s){
			return 0;
		}
		if(x>0){
			if(walls[y][x-1].e==1) 
				return 0;
		}
		if(x<5){
			if(walls[y][x+1].w==1)
		return 0;
		}
   }else{
	 if(walls[y][x].w==1 && walls[y][x].e == 1 ||  walls[y][x].n==1 && walls[y][x].s ){
		return 0;
	 }
	
	 	if(y>0){
			if(walls[y-1][x].s==1) 
				return 0;
		}
		if(y<5){
			if(walls[y+1][x].n==1) 
				return 0;
		}
	 } 
	 
	return 1;
}

/*finalPlaceWall places a final wall and updates walls matrix*/

void finalPlaceWall( uint8_t x, uint8_t y,struct Player *p, struct Wall (*walls)[6]){

	if(currentWallDirection==HORIZONTAL){
		walls[y][x].e=1;
		walls[y][x].w=1;
	}else{
		walls[y][x].n=1;
		walls[y][x].s=1;
	}

  if(turn==0){
		white.walls--;
  }else{
		red.walls--;
	}
	
  printVariable(37, 285, red.walls, White, Black);
  printVariable(195, 285, white.walls, White, Black);
	
  turn = 1 - turn;
  disable_timer(0);
  turn_time = 19;
  enable_timer(0);
  ChangeMode();
 	if(turn==0){
		move(white, red, Walls, Green);
  }else{
		move(red, white, Walls, Green);
	}
											 
}

/*controlPrevious: when you move a wall, it checsk if there was another wall there before (if yes it draws it again) */

void controlPrevious(uint8_t x, uint8_t y, struct Wall (*walls)[6]){

	if(walls[y][x].n ==1 || walls[y][x].s ==1){colorVerticalRectangle(37+32*x, 12+32*y);}
	if(walls[y][x].e ==1 || walls[y][x].w ==1){colorHorizontalRectangle( 12+32*x,37+32*y);}
	
	if(x>0){if(walls[y][x-1].e==1)colorHorizontalRectangle( 12+32*(x-1),37+32*y);}
	if(x<5){if(walls[y][x+1].w==1)colorHorizontalRectangle( 12+32*(x+1),37+32*y);}
	if(y>0){if(walls[y-1][x].s==1)colorVerticalRectangle(37+32*x, 12+32*(y-1));}
	if(y<5){if(walls[y+1][x].n==1)colorVerticalRectangle(37+32*x,12+32*(y+1));}

}

/* checKtrap is a  method that starts a recursion to check is players are trapped*/
int checkTrap(void){
	
  struct Wall tempWalls[6][6]; // current wall temporary placed
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t d = 0;

	uint8_t wasHR[7][7]={0}; // matrix taking track of Red's past positions
	uint8_t wasHW[7][7]={0}; // matrix taking track of White's past positions
	
	 for ( i = 0; i < 6; ++i) {
        for (j = 0; j < 6; ++j) {
            tempWalls[i][j] = Walls[i][j];
        }
    }
	 
		if(currentWallDirection==HORIZONTAL){
			tempWalls[currentWall.y][currentWall.x].e=1;
      tempWalls[currentWall.y][currentWall.x].w=1;
		}else{
		  tempWalls[currentWall.y][currentWall.x].n=1;
		  tempWalls[currentWall.y][currentWall.x].s=1;
		}
																		 
	if (recursiveSolve(white, white.posX, white.posY, wasHW, tempWalls)){ 
		d++; //white is not trapped
	}																				 
	if (recursiveSolve(red, red.posX, red.posY,wasHR, tempWalls)){ 
		d++; //red is not trapped
	}	

	if(d==2){
		return 0;
	}else{
		GUI_Text(13,240, "Player trapped", White, Black);
		GUI_Text(13,240, "              ", White, Black);
	  return 1;
	}

}
/* recursiveSolve is a recursive method checking if a player is  trapped*/

int recursiveSolve(struct Player p, uint8_t x, uint8_t y ,uint8_t (*wasH)[7], struct Wall tempWalls[6][6] ){
	
  if(p.color==1 && y==6) {
		return 1; //player 1 reached the end
	}
  if(p.color==0 && y == 0) {	
		return 1; //player 2 reached the end
	}
	
  if(wasH[y][x]==1) return 0; // the player has already been here

	wasH[y][x]=1; //the player has been here 
	
	if(x!=0){
		if(moLeft(x, y,tempWalls)&& recursiveSolve(p, x-1, y,wasH, tempWalls))
		return 1;
	}
	if(x!=6){
		if(moRight(x, y,tempWalls )&& recursiveSolve(p, x+1, y,wasH, tempWalls))
		return 1;
	}
	if(y!=0){
		if(moUp(x,y,tempWalls )&& recursiveSolve(p, x, y-1,wasH, tempWalls))
		return 1;
	}
  if(y!=6){
		if(moDown(x, y,tempWalls )&& recursiveSolve(p, x, y+1,wasH, tempWalls))
	return 1;
	}
	
	return 0;

}



void saveValues(volatile uint32_t *variable){

	if (turn == 0) {
    *variable |= ((uint32_t)0) << 24;
} else {
    *variable |= ((uint32_t)1) << 24;
}

	if(currentMode==WALL_PLACEMENT){
		*variable |= ((uint32_t)1) << 20;
	}else{
		*variable |= ((uint32_t)0) << 20;
	}

         	if(currentMode==WALL_PLACEMENT&&currentWallDirection==HORIZONTAL){
	*variable |= ((uint32_t)1) << 16;
	}else{
		*variable |= ((uint32_t)0) << 16;
	}

	if(currentMode==WALL_PLACEMENT){
		*variable |= ((uint32_t)currentWall.y) << 16;
	}else{
		if(turn==0){*variable |= ((uint32_t)white.posY )<< 16;
		}else{*variable |= ((uint32_t)red.posY )<< 16;}
	}

	if(currentMode==WALL_PLACEMENT){
		*variable |= currentWall.x;
	}else{
		if(turn==0){*variable |= white.posX ;
		}else{*variable |= (red.posX )<< 16;
		}
  }
        }

void saveOutOfTime(volatile uint32_t *variable) {
	
  *variable = 0;
	*variable |= 1 << 16;

}



/*EOF*/
