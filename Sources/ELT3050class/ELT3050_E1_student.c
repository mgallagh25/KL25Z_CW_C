/*
Create code that will blink leds 2 and 3 using the pwm attached to timer 0 with channels 0 and 1.  

The function prototypes exist; it will be your job to actually create them.  

void init_PWM0_0_1(int)  will initialize the PWM for timer 0 channels 0 and 1.  The
                         int argument is the period in ms


void PWM0_0_pw(int);    will set the pulse width timer 0 channel 0.  
                        int argument is the PW in ms
void PWM0_1_pw(int);         same as  above for channel 1.

 *
 */

int x=0,y=0;



#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
/**************Global Variables Here************************/

void init_PWM0_0_1(int);
void PWM0_0_pw(int);
void PWM0_1_pw(int);

int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

        
       
          init_PWM0_0_1(66);
          syst_tick_init();
//*************************************************************** 
	
	for(;;) {	 
		
		//you will need code here to select different pulse widths
  		
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

			
		        
//********Module/peripheral Configuration**************

     
     // Get the right value so that the argument is effectively in 1ms.
     

}

void PWM0_1_pw(int pw_tim_ms){
	
	 	
}

void PWM0_0_pw(int pw_tim_ms){
	
	 	
}


/***************************************************************
 *  This is code we discussed in class.  Use it as a starting point to create your functions
 *   For proper comments to this initialization see the functions in
 * my_timer_apps.c
 * 
 * This was renamed with a 'B' so as not to cause a compliation error
 * because it exists already in my_timer_apps.c*/

void init_PWM_0_0_B(int tim_us)

{

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
     SIM_SCGC5|=SIM_SCGC5_PORTC_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;   //>> Use new timer value here (timer 0,1 or 2)
     PORTC_PCR(1)=PORT_PCR_MUX(4);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
		       
			
		        
//********Module/peripheral Configuration**************

     
     TPM0_MOD = tim_us*24/16   ;					// this is another form of   Tof = fbus * mod / PS
     TPM0_SC = TPM_SC_CMOD(1)|TPM_SC_PS(4);	//>> Use new timer value here (timer 0,1 or 2) PS=16, .66ms /clk
     TPM0_C0SC=0X28;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C0V=00;						//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
}

void PWM_0_0_pw_B(int tim_us){
	
	TPM0_C0V=tim_us*24/16;			//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}
