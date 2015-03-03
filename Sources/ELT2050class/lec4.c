
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

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

   init_gpio_a(1,0);
   init_gpio_a(2,0);
   init_gpio_a(4,0);
   init_gpio_a(5,0);
   
   init_gpio_c(0,1);
   init_gpio_c(1,1);
   init_gpio_c(2,1);
   init_gpio_c(3,1);
 
 // turn off the LEDs
 
   gpo_c(0,1);
   gpo_c(1,1);
   gpo_c(2,1);
   gpo_c(3,1);
  
	
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		if(!gpi_a(1))gpo_c(0,0);
		else gpo_c(0,1);
		
		if(!gpi_a(2))gpo_c(1,0);
	    else gpo_c(1,1);
				
		if(!gpi_a(4))gpo_c(2,0);
	    else gpo_c(2,1);
		
		if(!gpi_a(5))gpo_c(3,0);
	    else gpo_c(3,1);

		delay_10us();

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
