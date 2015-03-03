
/*
 
Name      Program title
;-----------------------------------------------------------------------------
; PROGRAM FUNCTION: This program will turn on led 1 if pb1 is pushed.
it will turn off led 1 if button 2 is pushed pushed.  The led will stay on after
even after pb 1 is released.



;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Main:  initialize ios, then infinite loop.  Query pb1... 

delay_1s(): 

;-------------------------------------------------------------------------------
;Input/Output

pb1 : PTA1
pb2 : 
What are the input/outut ports you are using, which pins are doing what.

;------------------------------------------------------------------------------
;VARIABLES:

globals
x: analog to digital value



;*****************************************************************************

*/


#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"


/******* Prototype Function declarations ******/

    void delay_10us(void);

    void delay_1ms(void);
    	
    void delay_1s(void);

/**************Global Variables Here************************/

/***********************************************************/
int main(void) {
 
//*******Local Variable Declarations for main *****************

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

SIM_SCGC5=0b0000010000000000 ;   //  or 0xA00  turn on port A and C    


PORTB_PCR4=0b0000000100000010;    //0x100
PORTB_PCR5=0b0000000100000010;    //0x100

// outputs initializations

 GPIOC_PDDR = 0b00001111;
 GPIOA_PDDR &= ~0b110110;
 
 GPIOB_PDDR &= ~0b010000;
 GPIOB_PDDR |=  0b100000;
  
 // turn off the LEDs
 
 GPIOC_PDOR = 0b0001111;
	
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		if(!(GPIOA_PDIR & 0b10))GPIOC_PDOR = GPIOC_PDOR & ~0b1;
		
		if(!(GPIOA_PDIR & 0x00000004))GPIOC_PDOR = GPIOC_PDOR | 0b1;
		
		delay_1s();
		

		}
	return 0;
    }
    
    
 /********************************************************************************/
 /*  This is a routine that delays for approximately 10us.  It loops 20 times just 
  * keeping the CPU busy.  To get a longer delay you can increase the number of loops
  * or to create another routine that calls this one.
  * ******************************************************************************/
    void delay_10us(void){
    	int i=0;
    	
    	for(i=0;i<20;i++);
    	 
      }

    void delay_1ms(void){
    	int i=0;
    	    	
    	for(i=0;i<100;i++)delay_10us();
    	    	 
        }
    
    void delay_1s(void){
        	int i=0;
        	    	
        	for(i=0;i<1000;i++)delay_1ms();
        	    	 
            }
        
