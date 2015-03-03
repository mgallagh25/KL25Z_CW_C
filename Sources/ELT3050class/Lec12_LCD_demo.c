/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 

#include "my_lcd.c" /* include lcd declarations */
/**************Global Variables Here************************/

char numbuff[16];
/********************************   Function Prototypes*********************************/

void LCD_numchar_out(char);
void LCD_numint_out(int);
void LCD_mem_out(int);



int main(void) {
 
	//*******Local Variable Decalrations for main *****************
   int x=0;
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************

    



//Initialize the lcd
    LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
    
    sprintf(numbuff,"0123456789ABCDEF");

    //Initialize the ADC for the pot
         init_adc0();
         init_gpio_e(21,1);
        gpo_e(21,1);   //Turn on and off the high side switch
           
    syst_tick_init();       

// Convert the specific pins to the GPIO module.  Pins B18,B19, D1
     
	 LCD_send_byte(1,'a');
     LCD_pos_xy(5,1);
	 LCD_write_char('c');
	 LCD_write_string("hip hip hurray");    
	 
	 LCD_write_char('\f');     // clear the display
	 LCD_write_string("This is much easier now"); // write on the display
	 LCD_write_char('\f');     // clear the display
	 LCD_mem_out(0x20000004);
	 LCD_write_char('\f');     // clear the display
	LCD_write_string("ADC=")	;  
	
	for(;;) {	 
		
    x=adc0_value(0);
    
    LCD_pos_xy(5,0);
	  LCD_numint_out(x);
		
		while(sys_ticks<1000);
		sys_ticks=0;
		
		
	}
	
	return 0;
    }

void LCD_numchar_out(char ch){
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
     LCD_numchar_out(tmp);
     tmp=ch;
     tmp=tmp>>16;
     tmp=tmp&0x00ff;
     LCD_numchar_out(tmp);
     tmp=ch;
     tmp=tmp>>8;
     tmp=tmp&0x00ff;
     LCD_numchar_out(tmp);
     tmp=ch;
     tmp=tmp&0x00ff;
     LCD_numchar_out(tmp);
     return;
     
}

  
void LCD_mem_out(int loc){
     char x;                        
     x = *(char *)loc;      //prints out the location = value
     LCD_numint_out(loc);
     LCD_write_char('=');
     LCD_numchar_out(x);
      return;
     
}
