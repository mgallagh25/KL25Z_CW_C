/*
 This program will use the SCI comm port between to micros.
 
 Master will communicate the ADC value to the slave through the SCI
 
 Slave will display the ADC value on the LCD 
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 
#include "my_UART0_apps.c"   //for the SCI communication link


/**************Global Variables Here************************/


/********************************   Function Prototypes*********************************/



int main(void) {
 
	//*******Local Variable Decalrations for main *****************
   int x;
   char xLSBs,xMSBs;
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	 int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************

   
//Initialize the ADC for the pot
     init_adc0();
     init_gpio_e(21,1);
    gpo_e(21,1);   //Turn on and off the high side switch
// Initialize the SCI (UART0) as a master
    init_serial_master();
    
    syst_tick_init();       

// Convert the specific pins to the GPIO module.  Pins B18,B19, D1
   
	
	for(;;) {	 
		
    x=adc0_value(0);
    xMSBs=x>>8;
    xLSBs=x;
    
    serial_tx(0,xLSBs);
    serial_tx(1,xMSBs);
   	
		while(sys_ticks<1000);
		sys_ticks=0;
		
		
	}
	
	return 0;
    }

