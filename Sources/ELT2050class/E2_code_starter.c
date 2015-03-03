/*

This program exercises some functions you should be familiar with.  They are located in the
project headers folder in my_io_apps.c

init_keypad()   //initialize the keypad ios
getkey()
anykey()

init_PWM_0_0(period)  // initialize the PWM for timer 0 and channel 0 to have a period in micro seconds
PWM_0_0_PW(pulse width)  // set the pulse width in microseconds for timer 0, channel 0
 */

#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"

/**************Defines Here************************/

/******* Prototype Function declarations ******/

/**************Global Variables Here************************/


int main(void) {
 
//*******Local Variable Declarations for main *****************
  char x=0;
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
     pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL
//******User chip module initializations***************************************
     init_keypad();
     init_PWM_0_0(5000); 
        
//*************The infinte loop.  Could use while(1);	
	for(;;) {
	
		if(anykey()){
			x=getkey();
			while(anykey());
			delay_100us(50);
			
		}
		
		if(x=='A')PWM_0_0_pw(1000);
		
		
	}
	
    }

