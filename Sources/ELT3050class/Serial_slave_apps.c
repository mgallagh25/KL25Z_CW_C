/*
 This is the comment header for the my_UART0_apps.c file in the header folder.

This comment header describes how the functions in both the master and slave 
device work.  There are functions which should be used in the slave device 
and others in the master.

Master

void init_serial_master(void)
void serial_tx(char x, char y)    send the value y to the xth element in UART0_buffer[10]
char serial_rx(char x)  send back to the master the value in the xth element of UART0_buffer[10]

Slave

void init_serial_slave(void)    set the IRQ priority to 3 (lower priority than sys tick
void UART0_IRQHandler(void)    does not need to be invoked in the main program.  handles the commands
                               coming from the master as an interrupt.

The pin assignments for all the UART0 pins is this

A1      Rx      Alt2
A2      Tx      Alt2
A14     Rx      Alt3
A15     Tx      Alt3
B16     Rx      Alt3
B17     Tx      Alt3
D6      Rx      Alt3   <<use these
D7      Tx      Alt3   <<


These programs talk to eachother through the UART0_ comm port.  
There is an array in the slave device that simply is managed by the master device with the two functions

value serial_rx(i);      //returns the char value of the ith array element given in the argument
serial_tx(i, value); //writes the char value in the ith array element in the slave

The slave device has an interrupt function that waits for the master to tell it where and what to to
with the 10 element array UART0_buffer[10]. 

*******************  Functions  *****************************

              Master functions

void init_serial_master    set baud rate, normal UART0_ operation, 8 bits ,no parity
char serial_rx(char i);      //returns the char value of the ith array element given in the argument
void serial_tx(char i, char value); //writes the char value in the ith array element in the slave

              Slave functions
void init_serial_slave    set baud rate, normal UART0_ operation, 8 bits ,no parity, turn on interrupt with 
                   lower priority than the sys_tick
void UART0_IRQHandler(void)
   
       Has two main part: if a # is first sent by the master it tells the slave the master
       wants to READ a value in the UART0__reg[]
       
       If a * is sent it wants the slave to WRITE a value to a specific register.     
********************  Variable  ************************************************
 
  
char UART0_buffer[10] slave device only global array to store communiation data

*****************************************************************************
 
 */







#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 
#include "my_UART0_apps.c" 

/**************Global Variables Here************************/
/******* Prototype Function declarations ******/
/**************Global Variables Here************************/


int main(void) {
 
//*******Local Variable Declarations for main *****************
	
char i=0;
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************
  init_serial_slave();  
    syst_tick_init();
//*************The infinte loop.  Could use while(1);	
	for(;;) {
		
	 	 
	}
	
	return 0;
    }

