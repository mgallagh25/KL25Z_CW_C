/*   My_timer_apps.c

 
 
 */


/*************************     InitPWM_PORTXn_timer_Y_channel_Z      I************/
/* This routine will initialize a for PWM connected to PORTX where X is A,B,C,D or E.  The PWM resulting will be
 * based on a 1us tick period so this is the smallest division you will achieve.
 * You must follow these directions.  Use the table below to make the proper changes:
 * 
 * Determine which port and port pin you want to use  X = port,  n=pin
 * This will determine which timer and channel you need  timer = Y, Z= Channel
 * The initialization code below has several places that may need modification
 * 
       Mux ALT=3     ALT=4
PTA0	FTM0_CH5	
PTA1	FTM2_CH0	
PTA2	FTM2_CH1	         //Ex  X=A , n=2, Y=2, Z=1  (port=A, pin 2, timer2, channel 1)
PTA3	FTM0_CH0	
PTA4	FTM0_CH1	
PTA5	FTM0_CH2	
PTA12	FTM1_CH0	
PTA13	FTM1_CH1
*****************
PTB0	FTM1_CH0	
PTB1	FTM1_CH1	
PTB2	FTM2_CH0	
PTB3	FTM2_CH1	
PTB18	FTM2_CH0	
PTB19	FTM2_CH1	
**********************
PTC1				FTM0_CH0
PTC2				FTM0_CH1
PTC3				FTM0_CH2
PTC4				FTM0_CH3
PTC8	FTM0_CH4	
PTC9	FTM0_CH5	
********************************	
PTD0				FTM0_CH0
PTD1				FTM0_CH1
PTD2				FTM0_CH2
PTD3				FTM0_CH3
PTD4				FTM0_CH4
PTD5				FTM0_CH5
***************************
PTE20	FTM1_CH0	
PTE21	FTM1_CH1	
PTE22	FTM2_CH0	
PTE23	FTM2_CH1	
PTE24	FTM0_CH0	
PTE25	FTM0_CH1	
PTE29	FTM0_CH2	
PTE30	FTM0_CH3	
PTE31	FTM0_CH4	

***************************
To control the time for the PWM:

    frequency=24Mz for this chip  Tclk = 1/24MHz =  41.6 ns
    PS is the prescalor value set in this line below:
    
    TPM2_SC = TPM_SC_PS(7);    //divide by 128
    
    PS values from manual
    000 Divide by 1
    001 Divide by 2
    010 Divide by 4
    011 Divide by 8
    100 Divide by 16
    101 Divide by 32
    110 Divide by 64
    111 Divide by 128 
    
    PWM period = Tclk * PS * mod
 * */


volatile int T1,T2;


void InitPWM_PORTA2_timer_2_channel_1(void)     //>>Copy and paste the whole function for each PWM you need.  Use X, n, Y and Z to create the new name
		
{

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM2_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTA_PCR(2)=PORT_PCR_MUX(3);					//>> Use X n and mux value here.  Its mux 3 usually
		       
			
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM2_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM2_CONF = 0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM2_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM2_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM Counter and the timer prescalor.  Period = ps*mod/f.   
     
     TPM2_SC |= (TPM_SC_CMOD(1))|0x7;					//>> Use new Y value here (timer 0,1 or 2)
     TPM2_C1SC=0X28;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM2_C1V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
              
}
/*******************************************************************************************************************/
void InitPWM_PORTC1_timer_0_channel_0(void)

{

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
     SIM_SCGC5|=SIM_SCGC5_PORTC_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTC_PCR(1)=PORT_PCR_MUX(4);					//>> Use X n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
		       
			
		        
//********Module/peripheral Configuration**************

     TPM0_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM0_CONF = 0;								//>> Use new Y value here (timer 0,1 or 2)
     TPM0_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)
     TPM0_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
     TPM0_SC |= TPM_SC_CMOD(1);					//>> Use new Y value here (timer 0,1 or 2)
     TPM0_C0SC=0X28;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C0V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
              
}
/*******************************************************************************************************************/


void PWM_timer_0_channel_0_per(int per){
	 TPM0_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
	
}
void PWM_timer_0_channel_0_pw(int pw){
	 TPM0_C0V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}


/*******************************************************************************************************************/
void PWM_timer_2_channel_1_per(int per){
	 TPM2_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
	
}
void PWM_timer_2_channel_1_pw(int pw){
	 TPM2_C1V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}

void InitPWM_PORTD4_timer_0_channel_4(void)     //>>Copy and paste the whole function for each PWM you need.  Use X, n, Y and Z to create the new name
		
{

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTD_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTD_PCR(4)=PORT_PCR_MUX(4);					//>> Use X n and mux value here.  Its mux 3 usually
		       
			
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM0_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM0_CONF = 0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM0_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM0_MOD = 000;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM COunter
     TPM0_SC |= TPM_SC_CMOD(1);					//>> Use new Y value here (timer 0,1 or 2)
     TPM0_C4SC=0X28;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     TPM0_C4V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
              
}

void PWM_timer_0_channel_4_per(int per){
	 TPM0_MOD = per;							//>> Use new Y value here (timer 0,1 or 2)
	
}
void PWM_timer_0_channel_4_pw(int pw){
	 TPM0_C4V=pw;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
	 	
}


/* This function will activate the input capture function on timer n, channel Y  */
void init_input_capture_PORTA2_timer_2_channel_1(void)     //>>Copy and paste the whole function for each PWM you need.  Use X, n, Y and Z to create the new name
		
{

//*****System configuration********

//First, select the clock you want to use.  We use the PLLFLL clock. 
// these three lines should be the same for all timer initializations.
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
//Then enable the PORT and timer modules 
     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;				//>> Use new X value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM2_MASK;         		//>> Use new Y value here (timer 0,1 or 2)
     PORTA_PCR(2)=PORT_PCR_MUX(3);					//>> Use X n and mux value here.  Its mux 3 usually
		       
			
		        
//********Module/peripheral Configuration**************

//Clear the control registers to ensure that the counter is not running
     TPM2_SC = 0;                       		//>> Use new Y value here (timer 0,1 or 2)               
     TPM2_CONF = 0;								//>> Use new Y value here (timer 0,1 or 2)
//While the counter is disabled we can setup the prescaler

     TPM2_SC = TPM_SC_PS(7);   //				//>> Use new Y value here (timer 0,1 or 2)

     TPM2_MOD = 0xfffe;							//>> Use new Y value here (timer 0,1 or 2)
//Enable the TPM Counter and the timer prescalor.  Period = ps*mod/f.   
     
     TPM2_SC |= (TPM_SC_CMOD(1))|0x7;					//>> Use new Y value here (timer 0,1 or 2)
     TPM2_C1SC=TPM_CnSC_CHIE_MASK|0X0c;							//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)  input capture on rising and falling edges/enable interrupt
     TPM2_C1V=00;								//>> Use new Y and Z values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
     enable_irq (INT_TPM2-16);  //This sets the NVIC_ICPR and NVIC_ISER registers declaring the interrupt      
}
/*******************************************************************************************************************/

void FTM2_IRQHandler()
{
        //Clear the overflow mask if set.   According to the reference manual, we clear by writing a logic one!
		if(TPM2_C1SC & TPM_CnSC_CHF_MASK)
		{
			TPM2_C1SC |= TPM_CnSC_CHF_MASK;
		}
	//***********************************************
	
		TPM2_C1V=0;
		while(!(TPM2_C1SC & TPM_CnSC_CHF_MASK));
		T2=TPM2_C1V-T1;
		
		if(TPM2_C1SC & TPM_CnSC_CHF_MASK)
		{
			TPM2_C1SC |= TPM_CnSC_CHF_MASK;
		}
		
		
        //Enable TPM1 IRQ on the NVIC
     

}

