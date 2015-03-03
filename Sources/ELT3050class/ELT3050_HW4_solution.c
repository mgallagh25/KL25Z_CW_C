/*
*****************************************************************************
MG        KEYPAD and codes
;-----------------------------------------------------------------------------
This program will be to create code that take input from your keypad and put it 
into an array called code_enter[].  There will be a test for the last 4 button 
pushes to see if nAB# has been entered where n is between 1 and 4.  The board 
with then blink n leds.  If CD# is entered then the LEDs will stop blinking

CD#   stop blinking leds
nAB#  blink n leds  where n is between 0 and 4

;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Main:  will infinitely loop querying the keypad.  if ANYKEY() is pushed it will GETKEY()
GETKEY() will be followed by store_data(char,char *) and then test_dat(char *).  Depending on the return
value from test_dat leds will blink or not.

char anykey(void);  see my_interrupts.c
char getkey(void);  see my_interrupts.c

void store_data(char,char *) Enter data into and array called new_dat[].  Newest element goes
into [0] place.  All others get pushed forward.  4 characters only count.

char test_dat(char *)  test data with two secret codes:  

Return a 5 if "CD#" enters
Return a n if nAB# is entered  where n is between 0 and 4



;-------------------------------------------------------------------------------
;Input/Output

led1	ptc0
led2   	ptc1
led3	ptc2
led4	ptc3

keypad B0-B3, E0-E3


;------------------------------------------------------------------------------
;VARIABLES:

char new_dat[]  array that hold all key pushes
char key hold last key push

;*****************************************************************************

*/


/******* Prototype Function declarations ******/

void store_data(char,char *);
 
char test_dat(char *,char *);  


/**************Global Variables Here************************/










#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
//#include "my_interrupts.c" 

#include "my_lcd.c" /* include lcd declarations */
/**************Global Variables Here************************/

char new_dat[5];

int main(void) {
 
	//*******Local Variable Decalrations for main *****************
char key,x,j;

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

    
	
	for(;;) {	 
		
		if (anykey()==1){
			delay_ms(10);
			key=getkey();
			while(anykey())
			delay_ms(10);
			
			store_data(key,new_dat);
			}
		
		x=test_dat(new_dat,"#DC");   // Code has to be backwards
		
		if(x==1)leds(0);
		
		x=test_dat(new_dat,"#BA");   // Code has to be backwards
				
		if(x==1){
			j^=new_dat[3]-0x30;
			leds(j);
			delay_ms(200);
			
		}
				
		
			   	
		
	}
	
	return 0;
    }


void store_data(char key,char *p){
	signed char i;
	for(i=4;i>=0;i--){
		*(p+i+1)=*(p+i);
	}
	*p=key;
}


char test_dat(char *p,char *q){
	char i;
	
	for(i=0;i<3;i++){
		if(*(p+i)!=*(q+i))return 0;
	}
	
	 return 1;
}
