
/*
  
;*****************************************************************************
Matt Gallagher    Reading temp from the DS 1620 chip from Maxim
;-----------------------------------------------------------------------------
;   This program simply reads the temperature using a DS1620 chip made by Maxim. 
    It uses some new features for us: it uses a "bit bang" communication style
    in place  of a standard protocol.  See the documentation for the chip for 
    details on this "three wire" approach.  Some key facts
    - It uses increments of 0.5 degrees C so a 0x32 = 50 >> 25 C
    - Data is sent out LSB first
    - Data is read in LSB first
    
    The other new feature is using simple functions as define statements. See the
    define statements and see what that means
    
;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Front end functions:
int ds1620_read_temp(void);  return the temp in 2 x degrees C
                     - pullup the rst line so the chip is active
                     - send the 0xaa command to read the temp
                     - read the 9 bit data
  
void init_ds1620(void);  //initialize the DS1620 to read temps

Back end functions:

void ds1620_8out(char); //send an 8 bit data over the DQ line. Data read on clock rising edge
                      - for loop 8 times{
                      - pull clock low / wait
                      - update data  / wait
                      - pull clock high / wait  <<data latched in here              
                      -}

int ds1620_9in(void);     //read data over the DQ line
					- for loop 9 times{
					- pull clock low / wait
					- read bit and store in MSB place
					- shift data right / wait
					- pull clock high / wait
					}


;-------------------------------------------------------------------------------
;Input/Output

ds1620

1 DQ	8 Vdd =2.5-5V
2 clk	7 T high
3 !RST	6 T low
4 GND	5 T com

c12		DQ
c13  	clk
C16     !RST



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

*/


/******* Prototype Function declarations ******/

void init_ds1620(void);
int ds1620_write(char);
int ds1620_read_temp(void);
void ds1620_8out(char);
int ds1620_9in(void);
void LCD_numbyte_out(char);
void LCD_numint_out(int);
void LCD_mem_out(int);


/**************Global Variables Here************************/

char x=0,y=0;
char numbuff[16];
/***********************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_lcd.c" /* using XTAL and PLL does not work returns errors*/


#define bit_bang_dq_out_high()      GPIOC_PDOR|=1<<12
#define bit_bang_dq_out_low()       GPIOC_PDOR&=~(1<<12)
#define bit_bang_dq_ddr_output()      GPIOC_PDDR|=1<<12
#define bit_bang_dq_ddr_input()       GPIOC_PDDR&=~(1<<12)

#define bit_bang_dq_input()       (GPIOC_PDIR & (1<<12))>>12
#define bit_bang_rst_low()        GPIOC_PDOR&=~(1<<16)
#define bit_bang_rst_high()        GPIOC_PDOR|= 1<<16
#define bit_bang_clk_low()        GPIOC_PDOR&=~(1<<13)
#define bit_bang_clk_high()        GPIOC_PDOR|= 1<<13





int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.
          init_gpio_e(21,1);
          gpo_e(21,0);   //Turn on and off the high side switch
          gpo_e(21,1);   //Turn on and off the high side switch
          
          //Initialize the lcd
              LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);       		 
        
          sprintf(numbuff,"0123456789ABCDEF");
          init_ds1620();
          syst_tick_init();
//*************************************************************** 

	
	for(;;) {	 
		
		y=ds1620_read_temp();
		LCD_pos_xy(0,1);
		LCD_numbyte_out(y);
	}
	delay_ms(1000);
	return 0;
    }


/***************** init_DS1620 ******************************

This communication is a 3 wire connection so not an
existing communication protocol.  We will have to "bit bang"
using gpio ports.

Since we use C12 for DQ it must be both input and output
C13 is the clk and is always output.  C16 !RST is always output



*/
void init_ds1620(void)

{
	init_gpio_c(13,1);
	gpo_c(13,1);     //clk is true low so set high initially
	init_gpio_c(16,1);
	gpo_c(16,0);    //rst is true high to set low initially
	
	init_gpio_c(12,0);  //init as an input initially.
    bit_bang_rst_high(); 
    ds1620_8out(0xee);
    bit_bang_rst_low(); 
        

}

/***********************************************************************
 *                    ds1620_read_temp                              
 ***********************************************************************
read the temp from a DS1620

int ds1620_read_temp(void);  return the temp in 2 x degrees C
                     - pullup the rst line so the chip is active
                     - send the 0xaa command to read the temp
                     - read the 9 bit data 
                     - return data                       */
ds1620_read_temp(void)
{
	char i=0,j=0;
	int temp=0;
    bit_bang_rst_high(); 
    ds1620_8out(0xaa);
    delay_ms(1);
	temp=ds1620_9in();
	bit_bang_rst_low();  

	return temp;
}

/******************************************************************************************/
/*
void ds1620_8out(char); //send an 8 bit data over the DQ line. Data read on clock rising edge
                      - for loop 8 times{
                      - pull clock low / wait
                      - update data  / wait
                      - pull clock high / wait  <<data latched in here              
                      -}
*/
/******************************************************************************************/
void ds1620_8out(char dat){
	char i,j;
	bit_bang_dq_ddr_output();
	bit_bang_dq_out_high();
	delay_100us(2);
		for(i=0;i<8;i++){
			bit_bang_clk_low();
			delay_100us(2);
			
			if(dat&1)bit_bang_dq_out_high();   //check to see if lsb is a 1
			else bit_bang_dq_out_low();
			dat=dat>>1;                    //shift dat to the right
			
			delay_100us(2);
			bit_bang_clk_high();  
			delay_100us(4);
		}
}
/******************************************************************************************/
/******************************************************************************************/
/*int ds1620_9in(void);     //read data over the DQ line
					- for loop 9 times{
					- pull clock low / wait
					- read bit and store in MSB place
					- shift data right / wait
					- pull clock high / wait
					}
********************************************************************************************** */
int ds1620_9in(void){
	char i,j;
	int temp1=0;
	bit_bang_dq_ddr_input();
	delay_100us(2);
	    
	for(i=0;i<9;i++){
			bit_bang_clk_low();
			delay_100us(2);
			j=bit_bang_dq_input();
			temp1=temp1>>1;
			temp1|=j<<8;
			delay_100us(2);
			bit_bang_clk_high();  
			delay_100us(2);
		}
	return temp1;
}
/******************************************************************************************/
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
