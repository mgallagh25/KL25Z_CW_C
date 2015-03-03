
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


//*********Required includes.  Don't modify************************
#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

//*********** My includes *************************
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" 

/******* Prototype Function declarations ******/



/**************Global Variables Here************************/

/***********************************************************/
int main(void) {
 
//*******Local Variable Declarations for main *****************
int var1, var2, result;		

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

   init_gpio_b(18,1);
   init_gpio_b(19,1);
   init_gpio_d(1,1);
  

   syst_tick_init();   
   
   gpo_b(18,1);
   gpo_b(19,1);
   gpo_d(1,1);
   
   
             
	          
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		sys_ticks=0;
		gpo_b(18,0);
		gpo_b(19,0);
		gpo_d(1,0);
			while(sys_ticks<1000);
		
		sys_ticks=0;
		gpo_b(18,1);
		gpo_b(19,1);
		gpo_d(1,1);
			while(sys_ticks<1000);
		   
		
	}
	
	return 0;
    }

