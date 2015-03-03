
/*
  
;*****************************************************************************
Name      Program title
;-----------------------------------------------------------------------------
;Create a program to blink led 1 at 10 HZ and led 2 only when PB2 is held in.  
PB3 will blink led 3 at 5 Hz for 10 times. 

pb 3 will be an edge sensitive so led 3 will blink 10 time only once per button push

;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Main:  will infinitely loop queryiing the buttons with if statements
delay_ms(); delay x ms where x is the argument

;-------------------------------------------------------------------------------
;Input/Output

led1	ptc0
led2   	ptc1
led3	ptc2
led4	ptc3

pb1		pta1
pb2		pta2
pb3		pta4
pb4		pta5

;------------------------------------------------------------------------------
;VARIABLES:
char i: used for counting

;*****************************************************************************

*/


#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"


/******* Prototype Function declarations ******/



/**************Global Variables Here************************/

/***********************************************************/
int main(void) {
 
//*******Local Variable Declarations for main *****************
char i;		

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

  SIM_SCGC5=0xa00;					//0b0000101000000000  turn on port A and C    
  
  PORTC_PCR0=0x102;					//0b0000000100000010 activate the MUXs
  PORTC_PCR1=0x102;
  PORTC_PCR2=0x102;
  PORTC_PCR3=0x102;
  
  PORTA_PCR1=0x102;					//0b0000000100000010 activate the MUXs
  PORTA_PCR2=0x102;
  PORTA_PCR4=0x102;
  PORTA_PCR5=0x102;
    
  // outputs initializations
  
   GPIOC_PDDR = 0b00001111;		//set the outputs direction
   GPIOA_PDDR &= ~0b110110;		//set the input directions
   
   // turn off the LEDs
   
   GPIOC_PDOR = 0b0001111;
	
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		GPIOC_PDOR^=0x01;
		delay_ms(50);
		
		if(!(GPIOA_PDIR & 0x04))GPIOC_PDOR&=~0x02;
		else GPIOC_PDOR|=0x02;
		
		
		if(!(GPIOA_PDIR & 0x10)){
			for(i=0;i<10;i++){
				GPIOC_PDOR^=0x04;
				delay_ms(100);
							}
			while(!(GPIOA_PDIR & 0x10));
			
		}
		}
								
		
	return 0;
    }
