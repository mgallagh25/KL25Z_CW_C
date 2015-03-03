/*
 * main implementation: use this 'C' sample to create your own application
 *
 */




int x=0;

#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"   
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
/**************Global Variables Here************************/





int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

          //syst_tick_init(); 
          InitTPM1();
           adc0_init();   
         
         

//*************************************************************** 
	
     
     
	
	for(;;) {	 
		
		x=adc0_value(15);
	    while(tpm1_ticks<1000);
	    tpm1_ticks=0;
	}
	
	return 0;
    }
