
/*
  
     Grade with functioning program without header comments = 7/10
     Grade with functioning program with insufficient comments 8 or 9/10
     Grade with functioning program with sufficient comments 10/10
     Grade with my this rubric still present = 9/10
 
;*****************************************************************************
Name      Program title
;-----------------------------------------------------------------------------
; PROGRAM FUNCTION: In plain english, describe what your program does.  Be specific.
The goal in this header commentary is to make sure you convey the exact function
of your program.



;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Describe each function you are using, the input/output variables if any.  If there is a complicated
decision structure, describe that as best you can.

;-------------------------------------------------------------------------------
;Input/Output

What are the input/outut ports you are using, which pins are doing what.

;------------------------------------------------------------------------------
;VARIABLES:

global and local variables of note and what they are.


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
int data;		

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

  SIM_SCGC5=0b0000101000000000 ;   //  or 0xA00  turn on port A and C    
  
  PORTC_PCR0=0b0000000100000010;    //0x102    activate the MUX
  PORTC_PCR1=0b0000000100000010;    //0x102
  PORTC_PCR2=0b0000000100000010;    //0x102
  PORTC_PCR3=0b0000000100000010;    //0x102

  PORTA_PCR1=0b0000000100000010;    //0x100    activate the MUX for each pin
  PORTA_PCR2=0b0000000100000010;    //0x100
  PORTA_PCR4=0b0000000100000010;    //0x100
  PORTA_PCR5=0b0000000100000010;    //0x100
  
  // outputs initializations
  
   GPIOC_PDDR = 0b00001111;
   GPIOA_PDDR &= ~0b110110;
   
   // turn off the LEDs
   
   GPIOC_PDOR = 0b0001111;
	
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		if(!(GPIOA_PDIR & 0b00000000000000000000000000000010))GPIOC_PDOR = GPIOC_PDOR & 0b11111111111111111111111111111110;
		else GPIOC_PDOR = GPIOC_PDOR | 0b00000000000000000000000000000001;
		
		if(!(GPIOA_PDIR & 0x00000004))GPIOC_PDOR &= 0xfffffffd;
		else GPIOC_PDOR |= 0x00000002;
		
		if(!(GPIOA_PDIR & 0x00000010))GPIOC_PDOR &= ~0x00000004;
		else GPIOC_PDOR |= 0x00000004;
		
		if(!(GPIOA_PDIR & 0x00000020))GPIOC_PDOR &= ~0x00000008;
		else GPIOC_PDOR |= 0x00000008;

		}
	return 0;
    }
