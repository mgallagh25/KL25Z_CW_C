
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
char data;		

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

   SIM_SCGC5=0b0000001000000000 ;

  PORTA_PCR5=0b0000000100000010;    //0x102

   GPIOA_PDDR = 0b00000000;



	          
 
//*************The infinte loop.  Could use while(1);	
	for(;;) {

		if (GPIOA_PDIR& 0b00100000) data=1;      //PB4 connected to PTA5
		else data=0;

		}
	return 0;
    }
