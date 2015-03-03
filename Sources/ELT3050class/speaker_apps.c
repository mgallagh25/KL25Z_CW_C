/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 

#include "my_lcd.c" /* include lcd declarations */
/******Global variables *************/
      int T=0,Ton=0;

/*************  prototype function declarations ***************/
void beep_once(char,int);
/**********************************************************/
int main(void) {
 
	//*******Local Variable Decalrations for main *****************
		
	int per=25,i=0;
	char spkr=0;
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

 /*Turn on the systick interrupt.  This is setup to create an interrupt every 1ms and increment 
    a global variable sys_ticks.  */
    
    syst_tick_init();       
	
    per=25;
    
	for(;;) {	 

		beep_once(spkr,per);
		
		
		if(!gpi_a(1))spkr=1;
		
		if(!gpi_a(2))spkr=0;
		
		
	}
	
	return 0;
    }


void beep_once(char spkr,int per){

	
	
	if(spkr==0){
		gpo_a(12,0);
		return;
	}
	
	if(sys_ticks>T){
		T=sys_ticks+per;
		Ton=sys_ticks+1;
		gpo_a(12,1);
	}
	
	else if (sys_ticks>Ton){
		gpo_a(12,0);
	}
	
}
		
