/*
 
 This program tests the function of the serial communication 
 ports.  The slave will read the ADC and store tthe result 
 in the UART0_buffer.  The master will read the values
 
************ Functions****************
 
 init_serial_master()  initialize the chip as a slave

************  input output ports***********

master         slave
e6 tx           e7 rx
e7 rx           e6 tx
gnd         	gnd 

**************Variables ******************

char i MSBs of the ADC value from the slave
char j LSBs of the ADC value from the slave

int x = i<<8 | j  form the integer from the 8 bit characters 

 */
#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 
#include "my_UART0_apps.c"     //Need this include to use the UART applications
/**************Global Variables Here************************/
int x;
/******* Prototype Function declarations ******/
int main(void) {
//*******Local Variable Declarations for main *****************
char i,j;
//******Initial Chip Settings******************
//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal 

//******User chip module initializations***************************************
       init_serial_master();  
        		 
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
		i=serial_rx(9);
		j=serial_rx(10);
		x=(i<<8)|j;
	}
	return 0;
    }
