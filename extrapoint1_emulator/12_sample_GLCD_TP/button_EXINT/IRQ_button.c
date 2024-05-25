#include "button.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"
#include "../timer/timer.h"

extern int down_int0;
extern int down_key1;
extern int down_key2;

extern struct Player red;
extern struct Player white;
extern struct Wall Walls[6][6];
extern uint8_t turn;
extern int turn_time;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	down_int0=1;
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	down_key1=1;
	
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{ 
	down_key2=1;
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


