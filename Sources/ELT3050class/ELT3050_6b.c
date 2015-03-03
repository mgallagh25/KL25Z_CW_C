/*
Create code that will turn on an led if the analog value from the pot is 
greater than 1.5v and turn it off if it is less.  It will display the 
value being read from the ADC on the LCD.

 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_lcd.c" 

/**************Global Variables Here************************/

int x=0,y=0;
char numbuff[16];

void LCD_numbyte_out(char);     
void LCD_numint_out(int);  
void LCD_mem_out(int loc);

void init_input_capture_0_2(void);

int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

          syst_tick_init();
          sprintf(numbuff,"0123456789ABCDEF");
          //Initialize the lcd
              LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
          //Initialize the timer as input capture
              init_input_capture_0_2();
          //initialize an led
               init_leds();
               
  
//*************************************************************** 
	 
	   LCD_write_string("PW in ms ="); 

	   	   
	for(;;) {	 
		LCD_pos_xy(12,0);
		LCD_numint_out(x);
        if(x>2000)leds(0xf);
        else leds(0);
	}
	
	return 0;
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



/***************** init_input_capture_0_2 ******************************

Input capture initialization for use with PB4.  This is the timer 0, 
channel 2.  The input capture is initialized to cause an interrupt 
when a true high pulse is observed at pin A5.

The interrupt priority is set to 3 so the sys_tick will have higher
priority.

The time PS is 16 so timer clicks are 0.66us.  

PB4   pin A5

*/
void init_input_capture_0_2(void)

{

SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); 

SIM_SCGC5|=SIM_SCGC5_PORTA_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
SIM_SCGC6 |=SIM_SCGC6_TPM0_MASK;   //>> Use new timer value here (timer 0,1 or 2)
PORTA_PCR(5)=PORT_PCR_MUX(3);		//>> Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
     
	
      
//********Module/peripheral Configuration**************

TPM0_MOD = 0xfffe;					//>> Use largest value possible here to avoid overflowing too frequently
TPM0_SC = TPM_SC_CMOD(1)|TPM_SC_PS(4);	//>> Use new timer value here (timer 0,1 or 2) PS=16, .66ms /clk
TPM0_C2SC=0X4c;					//>> Use new timer and channel values here (timer 0,1 or 2, channels 0,1,2,3,4,5)
                              //
set_irq_priority (INT_TPM0-16, 3);
enable_irq (INT_TPM0-16);
}



/***********************************************************************
*                    FTM0_IRQHandler()                                 
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
//int T1=TPM0_C2V;

sys_ticks=0;

TPM0_C2SC&=~0X40;	//turn interrupt off	

delay_100us(2);  //if you need a debounce period

TPM0_STATUS=0x1ff;  //clear all the flags by writing 1 to them.

while((!(TPM0_STATUS&TPM_STATUS_CH2F_MASK))&&(sys_ticks<5000));  //wait for the flag to go high
x=sys_ticks;
//x=TPM0_C2V-T1

delay_100us(2);
TPM0_C2SC|=0X40;   //turn interrupt back on	
/*****You must include this step clearing the interrupt flag.  Don't remove*******/
TPM0_STATUS=0x1ff;    //clear the flag
/***********************************************************************************/
}



