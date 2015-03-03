/*
This program measures the length of a button push on button 4 using the input compare function
on the KL25Z.  it displays a number proportional to the time the button was pushed on the LCD.


Functions
/**********************************************************************
Main

Simply loops through and determines whether a new value has been input to the variable x which is the delay
of the last button push.  If there is a new value, show that value on the LCD.
/***********************************************************************
LCD_num_char_out(char), LCD_num_int_out(int)  are used to convert a numerical argument into ascii text which can
be viewed on the LCD.  It uses an array of the 16 hex digits in ascii form in num_buff[];

 /***************** init_input_capture_2_0 ******************************

Input capture initialization for use with PB4.  This is the timer 2, 
channel 0.  The input capture is initialized to cause an interrupt 
when a true high pulse is observed at pin A5.

The interrupt priority is set to 3 so the sys_tick will have higher
priority.

The time PS is 16 so timer clicks are 0.66us.  

PB4   pin A5
/***********************************************************************
 *                    FTMx_IRQHandler()                                 
 ***********************************************************************
There is only ONE interrupt for each timer.  Overflow and input capture 
interrupts are the same handler. This is the input capture interrupt.
If you want to use an overflow interrupt you will have to chose another
timer.

The input capture initialization is set to occur on rising edges, 
falling or both.

When an event is detected the interrupt occurs and the current value in
TPMx_CNT is latched into TPMx_CnV.

with the interrupt turned off the while loop waits for another edge. 
TPMx_CNT is again latched in the the TPMx_CnV.



*/







#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_lcd.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
/******* Prototype Function declarations ******/

;
void LCD_numbyte_out(char);
void LCD_numint_out(int);
void LCD_mem_out(int);


/**************Global Variables Here************************/


int x=0,y=0, T1,T2;
char numbuff[16];

int main(void) {
 
//*******Local Variable Decalrations for main *****************
	int y=0;
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//

          //Initialize the lcd
          LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);       		 
          sprintf(numbuff,"0123456789ABCDEF");

          init_input_capture_0_2();
//*************************************************************** 

     x=500;

		LCD_numint_out(x);
	for(;;) {	 
		
		
        if(!(x==y)){
        LCD_write_char('\f');
        LCD_pos_xy(0,0);
        LCD_numint_out(x);
        y=x;
        }
  		delay_100us(2);

		
	}
	
	return 0;
    }



/***************** init_input_capture_2_0 ******************************

Input capture initialization for use with PB4.  This is the timer 2, 
channel 0.  The input capture is initialized to cause an interrupt 
when a true high pulse is observed at pin A5.

The interrupt priority is set to 3 so the sys_tick will have higher
priority.

The time PS is 16 so timer clicks are 0.66us.  

PB4   pin A5

*/
void init_input_capture_2_0(void)

{

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM2_MASK;   //>> Use new timer value here (timer 0,1 or 2)
     PORTA_PCR(5)|=PORT_PCR_MUX(3);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
		       
			
		        
//********Module/peripheral Configuration**************

     TPM2_MOD = 0xfffe;					//>> Use largest value possible here to avoid overflowing too frequently
     TPM2_SC = TPM_SC_CMOD(1)|TPM_SC_PS(4);	//>> Use new timer value here (timer 0,1 or 2) PS=16, .66ms /clk
     TPM2_C0SC=0X4c;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
                                        //
     set_irq_priority (INT_TPM2-16, 3);
     enable_irq (INT_TPM2-16);
}
void init_input_capture_0_2(void)

{

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 
  
     SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
     SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;   //>> Use new timer value here (timer 0,1 or 2)
    
     init_gpio_a(5,0);
     
     PORTA_PCR(5)|=PORT_PCR_MUX(3);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
		       
			
		        
//********Module/peripheral Configuration**************

     TPM0_MOD = 0xfffe;					//>> Use largest value possible here to avoid overflowing too frequently
     TPM0_SC = TPM_SC_CMOD(1)|TPM_SC_PS(7);	//>> Use new timer value here (timer 0,1 or 2) PS=16, .66ms /clk
     TPM0_C2SC=0X4c;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
                                        //
     set_irq_priority (INT_TPM0-16, 3);
     enable_irq (INT_TPM0-16);
}



/***********************************************************************
 *                    FTMx_IRQHandler()                                 
 ***********************************************************************
There is only ONE interrupt for each timer.  Overflow and input capture 
interrupts are the same handler. This is the input capture interrupt.
If you want to use an overflow interrupt you will have to chose another
timer.

The input capture initialization is set to occur on rising edges, 
falling or both.

When an event is detected the interrupt occurs and the current value in
TPMx_CNT is latched into TPMx_CnV.

with the interrupt turned off the while loop waits for another edge. 
TPMx_CNT is again latched in the the TPMx_CnV.



 ************************************************************************/

void FTM0_IRQHandler()
{
	int k;
	
	TPM0_CNT=99;   //Clear CNT by writing any value to it.
	TPM0_C2SC&=~0X40;	//turn interrupt off	
    TPM0_SC|=0x80;     //Clear overflow flag
    TPM0_SC&=~0x80;     //Clear overflow flag
	delay_100us(10);  //if you need a debounce period

    k=0;	
	while((!(TPM0_STATUS&TPM_STATUS_CH2F_MASK))&&(TPM0_CNT<TPM0_MOD-2));  //wait for either event to occur
	//This k variable will prevent the while loop from permanently locking up

	if (TPM0_CNT>=(TPM0_MOD-2))x=000;
	else x=TPM0_CNT;
	
	delay_100us(10);
	TPM0_C2SC|=0X40;   //turn interrupt back on	
 /*****You must include this step clearing the interrupt flag.  Don't remove*******/
	TPM0_STATUS=0x1ff;    //clear the flag
/***********************************************************************************/
}


void LCD_numbyte_out(char ch){
     char tmp;
     tmp=ch;
     tmp=tmp>>4;
     LCD_write_char(numbuff[tmp]);
     ch=ch&0x0f;
     LCD_write_char(numbuff[ch]);
     return;
}


     
void LCD_numint_out(int ch){
     int tmp;
     
     tmp=ch;
     tmp=tmp>>24;
     LCD_numbyte_out(tmp);
     tmp=ch;
     tmp=tmp>>16;
     LCD_numbyte_out(tmp);
     tmp=ch;
     tmp=tmp>>8;
     LCD_numbyte_out(tmp);
     tmp=ch;
     tmp=tmp&0x00ff;
     LCD_numbyte_out(tmp);
     return;
     
}

  
void LCD_mem_out(int loc){
     int temp;                        
     temp = *(int *)loc;      //prints out the location = value
     LCD_numint_out(loc);
     LCD_write_char('=');
     LCD_numint_out(temp);
      return;
     
}



