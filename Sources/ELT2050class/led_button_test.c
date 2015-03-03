/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 

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

//Initialize speaker and turn it off
    init_gpio_a(12,1);
    gpo_a(12,0);

//Initialize the lcd
    LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
//Initialize the ADC for the pot
     adc0_init();
     init_gpio_e(21,1);
     gpo_e(21,1);   //Turn on and off the high side switch
    		 
    
    syst_tick_init();       

 
	

     
// Convert the specific pins to the GPIO module.  Pins B18,B19, D1
     
	 LCD_send_byte(1,'a');
     LCD_pos_xy(5,1);
	 LCD_write_char('c');
	 LCD_write_string("hip hip hurray");    
	
	for(;;) {	 
		
		sys_ticks=0;
		
		if(!(gpi_a(1)))gpo_c(0,0);
		
		else gpo_c(0,1);
			
		if(!(gpi_a(2)))gpo_c(1,0);
		else gpo_c(1,1);
		if(!(gpi_a(4)))gpo_c(2,0);
		else gpo_c(2,1);
		if(!(gpi_a(5)))gpo_c(3,0);
		else gpo_c(3,1);  
		
		x=adc0_value(0);
		if (x>2000)gpo_a(12,1);
		else gpo_a(12,0);

		while(sys_ticks<10);
			   	
		
	}
	
	return 0;
    }
