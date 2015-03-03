/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_timer_apps.c" /* include peripheral declarations      This one is for the PWM apps*/

/**************Global Variables Here************************/
/******* Prototype Function declarations ******/


/**************Global Variables Here************************/
int x,y;

int main(void) {
 
	//*******Local Variable Declarations for main *****************
	

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************
    
    

    init_adc0();
    init_gpio_e(21,1);
    gpo_e(21,1);   //Turn on and off the high side switch
    
    
    init_PWM_0_0(4100);
    

    
    x=19000;
    y=18000;
        
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
    
	x=adc0_value(0);   //0 thru 4080
	
	PWM_0_0_pw(x);
	
	
		
	}
	
	return 0;
    }
