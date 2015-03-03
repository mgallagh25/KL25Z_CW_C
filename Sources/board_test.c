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

char i=0;
int x=0;


char anykey(void);
char getkey(void);

int main(void) {
 
	//*******Local Variable Decalrations for main *****************
		
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************
// Turn on PBs and leds
    init_gpio_c(0,1);
    init_gpio_c(1,1);
    init_gpio_c(2,1);
    init_gpio_c(3,1);
    
    gpo_c(0,1);
    gpo_c(1,1);
    gpo_c(2,1);
    gpo_c(3,1);
    
    init_gpio_a(1,0);
    init_gpio_a(2,0);
    init_gpio_a(4,0);
    init_gpio_a(5,0);
    
//**********  Initialize the Keypad    ***********************
    
    init_gpio_b(0,1);
    init_gpio_b(1,1);
    init_gpio_b(2,1);
    init_gpio_b(3,1);
    
    gpo_b(0,1);
    gpo_b(1,1);
    gpo_b(2,1);
    gpo_b(3,1);
    
    init_gpio_e(0,0);
    init_gpio_e(1,0);
    init_gpio_e(2,0);
    init_gpio_e(3,0);

    
//Initialize speaker and turn it off
    init_gpio_a(12,1);
    gpo_a(12,0);

//Initialize the lcd
    LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
//Initialize the ADC for the pot
     adc0_init();
     init_gpio_e(21,1);
     gpo_e(21,1);   //Turn on and off the high side switch
    		 
    
    //syst_tick_init();       

 
	

     
// Convert the specific pins to the GPIO module.  Pins B18,B19, D1
     
	 LCD_send_byte(1,'a');
     LCD_pos_xy(5,1);
	 LCD_write_char('c');
	 LCD_write_string("hip hip hurray");    
	
	for(;;) {	 
		

		
		if(!(gpi_a(1)))gpo_c(0,0);
		
		else gpo_c(0,1);
			
		if(!(gpi_a(2)))gpo_c(1,0);
		else gpo_c(1,1);
		if(!(gpi_a(4)))gpo_c(2,0);
		else gpo_c(2,1);
		if(!(gpi_a(5)))gpo_c(3,0);
		else gpo_c(3,1);  
		
		x=adc0_value(0);
		if (x>2000)gpo_a(12,1);
		else gpo_a(12,0);

		//i=GPIOE_PDIR;
		if (anykey()==1){
			i=getkey();
			if (i=='0')LCD_write_char('\f');
			else LCD_write_char(i);
			while(anykey())
			delay_100us(5);
		}
		
		delay_100us(5);
			   	
		
	}
	
	return 0;
    }


char anykey(void){
	char temp=0;
	GPIOB_PDOR&=0xf0;
	delay_100us(1);
	temp=GPIOE_PDIR;
	if((temp&0x0f)==0x0f)return 0;
	else return 1;
}

char getkey(void){
	char temp=0;
	
	    GPIOB_PDOR&=0xf0;
	    GPIOB_PDOR|=0x07;
		delay_100us(1);
		
		temp=GPIOE_PDIR;
		temp&=0x0f;
		if(temp==0x07)return 'D';
		else if(temp==0x0b)return '#';
		else if(temp==0x0d)return '0';
		else if(temp==0x0e)return '*';

	    GPIOB_PDOR&=0xf0;
		GPIOB_PDOR|=0x0b;
		delay_100us(1);

		temp=GPIOE_PDIR;
		temp&=0x0f;
		if(temp==0x07)return 'C';
		else if(temp==0x0b)return '9';
		else if(temp==0x0d)return '8';
		else if(temp==0x0e)return '7';

	    GPIOB_PDOR&=0xf0;
		GPIOB_PDOR|=0x0d;
		delay_100us(1);
				
		temp=GPIOE_PDIR;
		temp&=0x0f;
		if(temp==0x07)return 'B';
		else if(temp==0x0b)return '6';
		else if(temp==0x0d)return '5';
		else if(temp==0x0e)return '4';

	    GPIOB_PDOR&=0xf0;
		GPIOB_PDOR|=0x0e;
		delay_100us(1);
				
		temp=GPIOE_PDIR;
		temp&=0x0f;
		if(temp==0x07)return 'A';
		else if(temp==0x0b)return '3';
		else if(temp==0x0d)return '2';
		else if(temp==0x0e)return '1';

}

/*
 *******************************************************************            
 This program assumes this connectivity to ports E and B
 If we input to the column pins, B0:3, then we can 
 read E0:3 as input.  Since the input pins are internally
 tied high, we can look for low values.

  Look at the board from front, orient the pins at the
  bottom, count left to right.
   
  
      e0  e1  e2  e3
    +---+---+---+---+
 b0 | 1 | 2 | 3 | A |
    +---+---+---+---+
 b1 | 4 | 5 | 6 | B |
    +---+---+---+---+
 b2 | 7 | 8 | 9 | C |
    +---+---+---+---+
 b3 | * | 0 | # | D |
    +---+---+---+---+ 

   1  2  3  4  5  6  7  8  
   o  o  o  o  o  o  o  o
   e0 e1 e2 e3 b0 b1 b2 b3


 * */
 

