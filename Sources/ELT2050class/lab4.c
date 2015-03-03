
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

void blink_50Hz(int);

/**************Global Variables Here************************/


/***********************************************************/
int main(void) {
 
//*******Local Variable Declarations for main *****************
int j=0;
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

   init_gpio_c(0,1);   //led 1 init   output
   init_gpio_a(1,0);   // pb1 init  input

    gpo_c(0,1);   //led 1 turning off
	
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		if(!gpi_a(1)){

		j=j^1;                   //Exlusive or a bit
		gpo_c(0,j);

		delay_100us(500);

		while(!gpi_a(1));

		delay_100us(500);
		}
		}
	return 0;
    }
    
   
void blink_50Hz(int count){
    int i;
	for(i=0;i<count;i++){
		gpo_c(0,0);
		delay_100us(1000);
		gpo_c(0,1);
		delay_100us(1000);
		
		
	}
}

