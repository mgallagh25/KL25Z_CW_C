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
/**************function prototype declarations********/


/**************Global Variables Here************************/

int x=0;
int sys_ticks_temp=0;


int main(void) {
 
	//*******Local Variable Decalrations for main *****************
		char i=0;
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************

	//          init_port_interrupt_a1(1);
	          
	          
	          init_gpio_a(1,0);
	          init_gpio_a(2,0);
	          init_gpio_a(4,0);
	          init_gpio_a(5,0);
	          	    
	          init_gpio_c(1,1);
	      	  gpo_c(1,1);
	          init_gpio_c(0,1);
	          gpo_c(0,1);
	for(;;) {	 
		
	  gpo_c(0,0);
	   delay_100us(1000);    //5Hz  200ms or Ton=100ms
	  gpo_c(0,1);
	  delay_100us(1000);
	  
	  if(!gpi_a(1)){
		  i^=1;
		  gpo_c(1,i);
		  while(!gpi_a(1));
		  delay_100us(500);
	  }
	  
		
	}
	
	return 0;
    }

/*****************************************************************************************************
 *              Init portn interrupt_nx(char edge)
 *              
 * These interrupts are activated by edges on parallel port pins.  
 * 
 * n:  a,d   only
 * 
 * x: number 0-31
 * 
 * edge: 0 turn off the interrupt
 *       1 rising edge
 *       2 falling edge
 *       3 rising and falling edge
 * The pin initialization function 
 * 
 * init_gpio_a is called to create an input pin.
 * 
 * To create another interrupt for pin d3, for example
 * 
 * replace  PORTA_PCR(1)  with PORTD_PCR(3)
 * 
 * 
 * */
