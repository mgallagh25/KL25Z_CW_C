/*
 * THIS FILE IS INCOMPLETE.  ITS MEANT TO HELP YOU GET STARTED
 
 main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 

#include "my_lcd.c" /* include lcd declarations */
/******Global variables *************/
      int T=0,Ton=0;

/*************  prototype function declarations ***************/
void beep_once(int);
//char anyKey(void);
//char getkey(void);
/**********************************************************/
int main(void) {
 
	//*******Local Variable Decalrations for main *****************
		
	int per=25,i=0;

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

 /*Turn on the systick interrupt.  This is setup to create an interrupt every 1ms and increment 
    a global variable sys_ticks.  */
    
    syst_tick_init();       
	
    
	for(;;) {	 

		//  All inputs to the keypad are low.  Monitor the outputs in port E.
	//gpo_b(0,0);
	//gpo_b(1,0);
	//gpo_b(2,0);
	//gpo_b(3,0);
	
	
	//for(i=0;i<5;i++){
		beep_once(50);
	//		}
	
	//sys_ticks=0;
	//while(sys_ticks<2000);
		
	}
	
	return 0;
    }

/********************************************************************************************
 * *******************    Beep once   *****************************************************
 * Subroutine to turn on and off the speaker one time.  
 * 
 * Variables 
 * int per       period of the speaker cycle
 * int T global variable that holds the temporary value of time for the current cycle to end
 * int Ton  global variable that holds the temporary value of the time the speaker should be on
 * sys_ticks global variable that keeps track of time.  Incremented every ms.     
 * 
 * Global variables must be defined outside of this function*/
void beep_once(int per){
	int T=0;
	sys_ticks=0;
	int hper=per>>1;
	
	while(sys_ticks<1000){
	
	if(sys_ticks<(T+hper)){
		gpo_a(12,1);
	}
	
	else if ((sys_ticks<(T+per))&&(sys_ticks>(T+hper))){
		gpo_a(12,0);
	}
	
	else T=sys_ticks;
	
	
	}
	
	
}
//***************************************************************
/*
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
}
*/
