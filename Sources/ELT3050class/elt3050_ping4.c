
/*
  
;*****************************************************************************
Name      Program title
;-----------------------------------------------------------------------------
;
;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 



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
		delay_100us(100);
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
pin A5  is the echo connected to timer 0 channel 2 input capture

The initialization:

C0 output, A5 input capture TPM0 channel 2


*/
void init_ping4(void)

{
	
/*******************	inititialize the input capture *********************/
	    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
		SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
		SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
	  
	     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
	     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;   //>> Use new timer value here (timer 0,1 or 2)
	     PORTA_PCR(5)=PORT_PCR_MUX(3);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
			        
//***Module/peripheral Configuration**************

	     TPM0_MOD = 0xfffe;					//>> Use largest value possible here to avoid overflowing too frequently
	     TPM0_SC = TPM_SC_CMOD(1)|TPM_SC_PS(7);	//>> Use new timer value here (timer 0,1 or 2) PS=128, 5.33us /click
	     TPM0_C2SC=0X0c;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	                                        //  No interrupt 
/*******************************************************/
//******initialize the trigger port as gpio*****************

	     init_gpio_c(0,1);
	     gpo_c(0,0);     //  bring the port low
}

/***********************************************************************
 *                    Ping 4                                 
 ***********************************************************************

Use the sys_tick timer to create pulses and to measure echo.
 ************************************************************************/

int ping4()
{
	static int T1,T2;
	TPM0_STATUS=0x1ff;
	sys_ticks=0;
	TPM0_CNT=0;
	gpo_c(0,1);    
	while(TPM0_CNT<7);
	gpo_c(0,0);
	
	TPM0_CNT=0;
		
//	delay_100us(200);
	TPM0_STATUS|=TPM_STATUS_CH2F_MASK;         //  Clear the fLg
	//while((!(TPM0_STATUS&TPM_STATUS_CH2F_MASK))&&(sys_ticks<5000));    //wait for the flag
	x=TPM0_CNT;
	
	//delay_100us(20);
}

