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

int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

        
          sprintf(numbuff,"0123456789ABCDEF");
          //Initialize the lcd
              LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
          //Initialize the ADC for the pot
               init_adc0();
               init_gpio_e(21,1);
               gpo_e(21,1);   //Turn on and off the high side switch
          //initialize an led
               init_leds();
               
  
//*************************************************************** 
	   y=0x20000008;
	   LCD_write_string("ADC value ="); 

	   	   
	for(;;) {	 
		x=adc0_value(0);
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
