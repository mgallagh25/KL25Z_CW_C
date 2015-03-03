/*
 * Generates a pulse train based on the AtD value from the potentiometer on the FRDM1.2 shield.
 * 
 * analog value AtD E20
 * Pulses     C1
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 
#include "my_timer_apps.c"

#include "my_lcd.c" /* include lcd declarations */
/**************Global Variables Here************************/

int x=0;

int main(void) {
 
	//*******Local Variable Decalrations for main *****************
		
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************

    
//Initialize speaker and turn it off
    init_gpio_a(12,1);
    gpo_a(12,0);

//Initialize the ADC for the pot
     adc0_init();
     init_gpio_e(21,1);
     gpo_e(21,1);   //Turn on and off the high side switch
    		 
     InitPWM_PORTC1_timer_0_channel_0();

		PWM_timer_0_channel_0_per(10000);
// Convert the specific pins to the GPIO module.  Pins B18,B19, D1
	
	for(;;) {	 
		

		
		x=adc0_value(0);
		x=x<<1;

		


		PWM_timer_0_channel_0_pw(x);		
		
		 delay_100us(10000);
		
	}
	
	return 0;
    }

