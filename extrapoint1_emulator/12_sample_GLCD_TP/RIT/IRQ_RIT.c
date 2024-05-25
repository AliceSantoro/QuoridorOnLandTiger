/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h" 
#include "../game/game.h"
#include "../timer/timer.h"
#include <stdio.h>

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down_int0=0;
volatile int down_key1=0;
volatile int down_key2=0;

volatile uint32_t variable;

volatile int selected_dir=-1;

enum Gamemode currentMode = PLAYER_PLACEMENT;

enum WallDirection currentWallDirection= HORIZONTAL;

extern struct Player red;
extern struct Player white;
extern struct Wall Walls[6][6];
extern uint8_t turn;
extern volatile int turn_time;
extern volatile uint8_t victory; 

struct CurrentWall currentWall;

void RIT_IRQHandler (void)
{					
	static int J_select=0;
	static int J_up = 0;
	static int J_left =0;
	static int J_right=0;
  static int J_down =0;

	if(victory==1){
		
		disable_timer(0);
		LCD_Clear(Black); 
		
		if(turn==0){
			GUI_Text(20, 160, (uint8_t *) "The winner is Red", White, Black);
		}else{
			GUI_Text(20, 160, (uint8_t *) "The winner is White", White, Black);
		}
		
		disable_RIT(); 
		return;
	}

	if ((LPC_GPIO1->FIOPIN & (1 << 25)) == 0) {
		
    /* Joytick J_select pressed p1.25*/
    J_select++;
   
    switch (J_select) {
        case 1:
            if (currentMode == PLAYER_PLACEMENT) {
                switch (selected_dir) {
                    case 0:
                        //  down
                        if (turn == 1) {
                            if (movDown(red, white, Walls)) {
                                turn = 1 - turn;
                                changeDown(&red);
															  saveValues(&variable);
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(white, red, Walls, Green);
                            }
                        } else {
                            if (movDown(white, red, Walls)) {
                                turn = 1 - turn;
                                changeDown(&white);
															  saveValues(&variable);
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(red, white, Walls, Green);
                            }
                        }
                        break;

                    case 1:
                        // left
                        if (turn == 1) {
                            if (movLeft(red, white, Walls)) {
                                changeLeft(&red);
															  saveValues(&variable);
                                turn = 1 - turn;
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(white, red, Walls, Green);
                            }
                        } else {
                            if (movLeft(white, red, Walls)) {
                                changeLeft(&white);
															  saveValues(&variable);
                                turn = 1 - turn;
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(red, white, Walls, Green);
                            }
                        }
                        break;

                    case 2:
                        //up
                        if (turn == 1) {
                            if (movUp(red, white, Walls)) {
                                changeUp(&red);
															  saveValues(&variable);
                                turn = 1 - turn;
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(white, red, Walls, Green);
                            }
                        } else {
                            if (movUp(white, red, Walls)) {
                                changeUp(&white);
															  saveValues(&variable);
                                turn = 1 - turn;
                                disable_timer(0);
														   	enable_timer(0);
                                turn_time = 19;
                                move(red, white, Walls, Green);
                            }
                        }
                        break;

                    case 3:
                        // right
                        if (turn == 1) {
                            if (movRight(red, white, Walls)) {
                                changeRight(&red);
															  saveValues(&variable);
                                turn = 1 - turn;
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(white, red, Walls, Green);
                            }
                        } else {
                            if (movRight(white, red, Walls)) {
                                changeRight(&white);
															  saveValues(&variable);
                                turn = 1 - turn;
                                disable_timer(0);
                                turn_time = 19;
															  enable_timer(0);
                                move(red, white, Walls, Green);
                            }
                        }
                        break;

                    default:
                        //no direction 
                        break;
                }
            } else {
                if (placeWall(currentWall.x, currentWall.y, white, Walls)) 
									{	
										finalPlaceWall(currentWall.x, currentWall.y, &white, Walls);
								    saveValues(&variable);
									}
            }

            break;

        default:
            break;
    }
	} else {
    J_select = 0;
	}

 
		
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		
		/* Joytick J_Down pressed p1.26*/
		
		J_down++;
		
		
		switch(J_down){
			case 1:
        if(currentMode==PLAYER_PLACEMENT){
					selected_dir=0;
				}else{
					if(currentWallDirection==HORIZONTAL){
						cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
					}else{
						cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y);
					}
					
					controlPrevious(currentWall.x, currentWall.y, Walls);
					
					 if(currentWall.y<5) currentWall.y++;
				if(turn==0){
					placeWall(currentWall.x,currentWall.y, white, Walls);
				}else{
				placeWall(currentWall.x,currentWall.y, red, Walls);
				}
			}
				
				break;
			default:
				break;
		}
	 }
	else{
			J_down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick J_left pressed p1.27*/
		
		J_left++;
		
		switch(J_left){
			case 1:
        if(currentMode==PLAYER_PLACEMENT){
					selected_dir=1;
				}else{
					if(currentWallDirection==HORIZONTAL){
						cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
        }else{
				cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y);
				}
				
				controlPrevious(currentWall.x, currentWall.y, Walls);
				
					if(currentWall.x>0) currentWall.x--;
				
					if(turn==0){
						placeWall(currentWall.x,currentWall.y, white, Walls);
					}else{
						placeWall(currentWall.x,currentWall.y, red, Walls);
					}
				}
				
				break;
			default:
				break;
		}
	}
	else{
			J_left=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick J_right pressed p1.28*/
		
		J_right++;
		
		
		switch(J_right){
			case 1:
					if(currentMode==PLAYER_PLACEMENT){
						selected_dir=3;
					}else{
					if(currentWallDirection==HORIZONTAL){
						cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
					}else{cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y)
					;}
					controlPrevious(currentWall.x, currentWall.y, Walls);
						if(currentWall.x<5) currentWall.x++;
					  if(turn==0){placeWall(currentWall.x,currentWall.y, white, Walls);
					}else{
						placeWall(currentWall.x,currentWall.y, red, Walls);
					}
					}			
				break;
			  default:
				break;
		}
	}
	else{
			J_right=0;
	}
	
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick J_up pressed p1.29*/
		
		J_up++;	
		
		switch(J_up){
			case 1:
				
          if(currentMode==PLAYER_PLACEMENT){
						selected_dir=2;}else{
		
					if(currentWallDirection==HORIZONTAL){
						cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
          }else{cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y);
					}
					
					controlPrevious(currentWall.x, currentWall.y, Walls);
					if(currentWall.y>0) currentWall.y--; 
					if(turn==0){placeWall(currentWall.x,currentWall.y, white, Walls);}else{
						placeWall(currentWall.x,currentWall.y, red, Walls);
						}
					}
					
				break;
			default:
				break;
		}
	}
	else{
			J_up=0;
	}

	/* int0 management */
	
	if(down_int0!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			
			down_int0++;	
			
		 NVIC_EnableIRQ(EINT2_IRQn);              /* enable irq in nvic                 */
	   NVIC_SetPriority(EINT2_IRQn, 1);				 /* priority, the lower the better     */
     NVIC_EnableIRQ(EINT1_IRQn);              /* enable irq in nvic                 */
	   NVIC_SetPriority(EINT1_IRQn, 2);	
			
			switch(down_int0){
				case 2:	
					
				 enable_timer(0);
	       startGame();
	       move(white, red, Walls, Green);
				
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_int0=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_key1!=0){ 
		
	 if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
				
			down_key1++;			
				
			switch(down_key1){
				case 2:	
				  if(currentMode==PLAYER_PLACEMENT){ 
						if (turn == 0) {
						move(white, red, Walls, Black);
          } else {
						move(red, white, Walls, Black);
          }
          }else{
						if(currentWallDirection==HORIZONTAL){
							cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
						}else{
							cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y);
						}
					controlPrevious(currentWall.x, currentWall.y, Walls);}	
	        ChangeMode();					
	        if (currentMode == WALL_PLACEMENT) { 
		
					currentWall.x=3;
					currentWall.y=2;
		    	currentWallDirection = HORIZONTAL;
					
          if (turn == 0) {
						if(white.walls==0){
							GUI_Text(13,240,"No walls available, m_t_t", White, Black);
						  GUI_Text(13,240,"                         ", White, Black);
						}
              placeWall(3, 2, white, Walls);
					} else {
						if(red.walls==0){
							GUI_Text(13,240,"No walls available, m_t_t", White, Black);
							GUI_Text(13,240,"                         ", White, Black);}
            placeWall(3, 2, red, Walls);}}else{
							
          if (turn == 0) {
						move(white, red, Walls,Green); 
          } else {
						move(red, white, Walls,Green);
						}
						}

					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_key1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_key2!=0){ 
		
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			
			down_key2++;				
			switch(down_key2){
				
				case 2:	
				if(currentMode==WALL_PLACEMENT){
					if(currentWallDirection==HORIZONTAL){
					cancelHorizontalRectangle(12+32*currentWall.x, 37+32*currentWall.y);
          }else{
						cancelVerticalRectangle(37+32*currentWall.x,12+32*currentWall.y);
					}
        controlPrevious(currentWall.x, currentWall.y, Walls);
				changeWallDirection();
				if(turn==0){
					placeWall(currentWall.x, currentWall.y, white, Walls);
				}else{
					placeWall(currentWall.x, currentWall.y,red,Walls);
				}
				}
				
				
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_key2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	

	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
