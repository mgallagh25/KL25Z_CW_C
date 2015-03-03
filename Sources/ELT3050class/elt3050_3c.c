
/*
  
;*****************************************************************************
Name      Program title
;-----------------------------------------------------------------------------
;Create a program to blink led 1 at 10 HZ and led 2 only when PB2 is held in.  
PB3 will blink led 3 at 5 Hz for 10 times. 

pb 3 will be an edge sensitive so led 3 will blink 10 time only once per button push

;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Main:  will infinitely loop queryiing the buttons with if statements
Sys_ticks interrupt: uses the overflow interrupt of the sys_tick to keep track of time.

;-------------------------------------------------------------------------------
;Input/Output

led1	ptc0
led2   	ptc1
led3	ptc2
led4	ptc3

pb1		pta1
pb2		pta2
pb3		pta4
pb4		pta5

;------------------------------------------------------------------------------
;VARIABLES:
char i: used for counting
int sys_ticks: volatile variable for the systicks interrupt.  Increments every 1ms

;*****************************************************************************

*/


/******* Prototype Function declarations ******/

void init_ping4(void);
int ping4(void);


/**************Global Variables Here************************/

int x=0,y=0;
/***********************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/



int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

        
       
          init_ping4();
          syst_tick_init();
//*************************************************************** 
	
     
     x=500;
	
	for(;;) {	 
		
  		x=ping4();
		delay_100us(2);
		//PWM_0_0_pw(x);
		
	}
	
	return 0;
    }


/***************** init_ping4 ******************************

This will use a 4 pin ping sensor with a level shifter.  The 
ping sensors operate at 5V.  

Operation of a ping:  20us approx pulse to the trigger pin.
                      Wait for echo.  PW of echo is proportional
                      to the distance to the ping.

Each level shifter: uses a 2n7000 nfet and two approx 10k resistors

pin out 2n7000: flat side down,pins facing towards you DGS

Level shifter connections
R1  	~10k
R2  	~10k
Q1(DGS)	2n7000
vdd  (3v)
vcc  (5v)
sig3
sig5

4 nodes connected to: 
n1: R1, Q1D, sig5
n2: R2, Q1G, vdd
n3: R2, sig3
n4: R1, vcc

The FRDM connections

pin C0   is the trigger
pin A5  is the echo

The initialization:

C0 output, A5 input


*/
void init_ping4(void)

{
   gpioa_init(5,0);
   gpioc_init(0,1);
}

/***********************************************************************
 *                    Ping 4                                 
 ***********************************************************************

Use the sys_tick timer to create pulses and to measure echo.
 ************************************************************************/

int ping4()
{
	static int T1,T2;
	
	sys_ticks=TPM0_CNT=0;

	delay_100us(200);
	TPM0_STATUS|=TPM_STATUS_CH2F_MASK;
	while((!(TPM0_STATUS&TPM_STATUS_CH2F_MASK));
	x=TPM0_CNT;
	
	delay_100us(20);
 /*****You must include this step clearing the interrupt flag.  Don't remove*******
	TPM0_SC|=0x80;
	TPM0_STATUS|=TPM_STATUS_CH2F_MASK|TPM_STATUS_TOF_MASK;
/***********************************************************************************
}
*/

void FTM0_IRQHandler()
{
	
	sys_ticks=0;
	TPM0_C2SC&=~0X40;	//turn interrupt off	

	delay_100us(10);  //if you need a debounce period

	TPM0_STATUS=0x1ff;
	
	while((!(TPM0_STATUS&TPM_STATUS_CH2F_MASK))&&(sys_ticks<5000));
	x=sys_ticks;

	
	delay_100us(10);
	TPM0_C2SC|=0X40;   //turn interrupt back on	
 /*****You must include this step clearing the interrupt flag.  Don't remove*******/
	TPM0_STATUS=0x1ff;    //clear the flag
/***********************************************************************************/
}



