/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

int x=0,y=0;



#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
/**************Global Variables Here************************/

void init_PWM0_0_1(int);
void PWM0_1_pw(int);
void PWM0_0_pw(int);


int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

        init_pbs();
       
          init_PWM0_0_1(66);
          syst_tick_init();
//*************************************************************** 
	
     
     x=500;
	
	for(;;) {	 
		
		if(!gpi_a(1)){
			x=0;
			y=65;
			
		}
		else if(!gpi_a(2)){
			x=15;
			y=45;
		}
		else if(!gpi_a(4)){
				x=45;
				y=15;
				}
		else if(!gpi_a(5)){
				x=65;
				y=0;
				}
  		delay_100us(2);
		PWM0_0_pw(x);
		PWM0_1_pw(y);
		
	}
	
	return 0;
    }


/***************************************************************
 * For proper comments to this initialization see the functions in
 * my_timer_apps.c
 * 
 * This was renamed with a 'B' so as not to cause a compliation error
 * because it exists already in my_timer_apps.c*/

void init_PWM0_0_1(int tim_ms)

{

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
     SIM_SCGC5|=SIM_SCGC5_PORTC_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;   //>> Use new timer value here (timer 0,1 or 2)
     PORTC_PCR(1)=PORT_PCR_MUX(4);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
     PORTC_PCR(2)=PORT_PCR_MUX(4);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
		       
			
		        
//********Module/peripheral Configuration**************

     
     // Get the right value so that the argument is effectively in 1ms.
     
     
     
     TPM0_MOD = tim_ms*24000/128;  //Tof = mod*fbus PSmod;//>> Use new timer value here (timer 0,1 or 2)
     TPM0_SC = TPM_SC_CMOD(1)|TPM_SC_PS(7);	//>> Use new timer value here (timer 0,1 or 2) PS=128, 5.33us /clk
     TPM0_C0SC=0X28;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)

     TPM0_C0V=00;						//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C1SC=0X28;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C1V=00;						//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)


}

void PWM0_1_pw(int pw_tim_ms){
	
	TPM0_C1V=pw_tim_ms*24000/128;			//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}

void PWM0_0_pw(int pw_tim_ms){
	
	TPM0_C0V=pw_tim_ms*24000/128;			//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}

