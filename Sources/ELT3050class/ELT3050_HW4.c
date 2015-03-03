/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 


/**************Global Variables Here************************/

char i=0;
int x=0;

int main(void) {
 
	//*******Local Variable Decalrations for main *****************
		
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************
// Turn on PBs and leds
    init_gpio_c(0,1);
    init_gpio_c(1,1);
    init_gpio_c(2,1);
    init_gpio_c(3,1);
    
    gpo_c(0,1);
    gpo_c(1,1);
    gpo_c(2,1);
    gpo_c(3,1);
    
    init_gpio_a(1,0);
    init_gpio_a(2,0);
    init_gpio_a(4,0);
    init_gpio_a(5,0);
    
//**********  Initialize the Keypad    ***********************
    
    init_gpio_b(0,1);
    init_gpio_b(1,1);
    init_gpio_b(2,1);
    init_gpio_b(3,1);
    
    gpo_b(0,1);
    gpo_b(1,1);
    gpo_b(2,1);
    gpo_b(3,1);
    
    init_gpio_e(0,0);
    init_gpio_e(1,0);
    init_gpio_e(2,0);
    init_gpio_e(3,0);

    

 
	
	for(;;) {	 
		

		//i=GPIOE_PDIR;
		if (anykey()==1){
			i=getkey();
			while(anykey())
			delay_100us(5);
		}
		
		delay_100us(5);
			   	
		
	}
	
	return 0;
    }
