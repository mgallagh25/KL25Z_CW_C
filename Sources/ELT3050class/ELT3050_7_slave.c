/*
 
 This program tests the function of the serial communication ports.  The slave
 will read the ADC and store tthe result in the UART0_buffer.  The master
 will read the values
 *********** Functions **********************
 init_serial_slave()  initialize the chip as a slave
 init ADC()
 ADC value()
 ************  input output ports***********
master         slave
e6 tx           e7 rx
e7 rx           e6 tx
gnd         	gnd 
**************Variables ******************
char i MSBs of the ADC value
char j LSBs of the ADC value
int x = i<<8 | j  form the integer from the 8 bit characters  */
#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 
#include "my_UART0_apps.c" 
/**************Global Variables Here************************/
/******* Prototype Function declarations ******/
int main(void) {
//*******Local Variable Declarations for main *****************
int x;
char i,j;
//******Initial Chip Settings******************
//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************
       init_serial_slave();  
    //Initialize the ADC for the pot
         init_adc0();
         init_gpio_e(21,1);
         gpo_e(21,1);   //Turn on and off the high side switch
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		x=adc0_value(0);
		i=x>>8;
		j=x;
		UART0_buffer[9]=i;
		UART0_buffer[10]=j;
		delay_ms(500);
	}
	return 0;
    }
