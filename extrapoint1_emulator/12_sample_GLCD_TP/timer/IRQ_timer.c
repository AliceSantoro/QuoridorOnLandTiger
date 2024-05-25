/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h>
#include "../game/game.h"
#include "../RIT/RIT.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int turn_time = 19; /* turn period is 20 second */
extern uint8_t turn; /* turn ==0 -> player whiten turn ==1 -> player red */
extern volatile uint32_t variable; /* move saving */
extern enum Gamemode currentMode; /* PLAYER_PLACEMENT mode or WALL_PLACEMENT */
extern enum WallDirection currentWallDirection; 
extern struct Player red; 
extern struct Player white;
extern struct Wall Walls[6][6];
extern struct CurrentWall currentWall; /* position of the possible wall to be placed */

void TIMER0_IRQHandler (void)
{   
	  
	  if(turn_time==9) {
		GUI_Text(104, 277, (uint8_t *) "   ", White, Black); /* to see the correct turn_time */
		}

		printVariable(104, 277, turn_time, White, Black);
		
	  turn_time--;
	
	   if(turn_time == 0){
			 
	  saveOutOfTime(&variable);
		turn_time=20;
  
		if(currentMode==PLAYER_PLACEMENT){ 
			
			if (turn == 0) {
				move(white, red, Walls, Black);
      } else{
				move(red, white, Walls, Black);}	
    }else{
			if(currentWallDirection==HORIZONTAL){
				cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
      }else{
				cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y);
			}

			controlPrevious(currentWall.x, currentWall.y, Walls);
		}
			
			turn = 1 - turn;  // Toggle the value of turn between 0 and 1
		 
			if (turn == 0) {
				move(white, red, Walls,Green); 
			} else {
				move(red, white, Walls,Green);
			 }}
	
     LPC_TIM0->IR = 1;			/* clear interrupt flag */
     return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
