
/*
  
     Grade with functioning program without header comments = 7/10
     Grade with functioning program with insufficient comments 8 or 9/10
     Grade with functioning program with sufficient comments 10/10
     Grade with my this rubric still present = 9/10
 
;*****************************************************************************
Name      Program title
;-----------------------------------------------------------------------------
; PROGRAM FUNCTION: In plain english, describe what your program does.  Be specific.
The goal in this header commentary is to make sure you convey the exact function
of your program.



;------------------------------------------------------------------------------
;SUBROUTINES/FUNCTIONS/INTERRUPT ROUTINES: 

Describe each function you are using, the input/output variables if any.  If there is a complicated
decision structure, describe that as best you can.

;-------------------------------------------------------------------------------
;Input/Output

What are the input/outut ports you are using, which pins are doing what.

;------------------------------------------------------------------------------
;VARIABLES:

global and local variables of note and what they are.


;*****************************************************************************

*/


#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" 

/******* Prototype Function declarations ******/
void SetNextState(void);

/******* Global variables *********************/
char state=1;
char last_state;
int time_remaining;

/***********************************************************/
int main(void) {
 
//*******Local Variable Declarations for main *****************


//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

//******User chip module initializations***************************************

	syst_tick_init();

   init_gpio_a(1,0);
   init_gpio_a(2,0);
   init_gpio_a(4,0);
   init_gpio_a(5,0);

   init_gpio_c(0,1);
   init_gpio_c(1,1);
   init_gpio_c(2,1);
   init_gpio_c(3,1);
	          
   gpo_c(0,1);
   gpo_c(1,1);
   gpo_c(2,1);
   gpo_c(3,1);
//*************The infinte loop.  Could use while(1);	
     for(;;){
   		 SetNextState();
    	}
	}
   //*****************************************************************/

   void SetNextState(void)
   {



	    
         switch( state ){			      //Switch specifies which variable controls the "case" 
             case 1: // Green light
                {
                GPIOC_PDOR = ~state;
                	
                if (!gpi_a(1)){
                last_state=state;
                time_remaining=6000-sys_ticks;
                state=4;
                }                	                              
                  
                if(sys_ticks>6000){
                	sys_ticks=0;
                	state=2;
                }
                }
              break;
           

             case 2: // yellow light
                {
                GPIOC_PDOR = ~state;
   
                	                	
                if (!gpi_a(1)){
                last_state=state;
                time_remaining=3000-sys_ticks;
                state=5;
                }                	                              
                if(sys_ticks>3000){
                	sys_ticks=0;
                	state=3;
                }
                
                 }
              break;

             case 3: // red light
                {
                GPIOC_PDOR = ~state;
               
                	                	
                	                     
                if (!gpi_a(1)){
                	last_state=state;
                	time_remaining=6000-sys_ticks;
                	state=4;
                }
                if(sys_ticks>6000){
                	sys_ticks=0;
                	state=1;
                }
                
                 }
              break;
             case 4: // finish last state
                {
                GPIOC_PDOR = ~last_state;
                              	                	
                	                              
                if(sys_ticks>time_remaining){
                	sys_ticks=0;
                	state=5;
                	
                }
                
                 }
              break;
             case 5: // walk
                {
                GPIOC_PDOR = ~state;
                              	                	
                	                              
                if(sys_ticks>6000){
                	sys_ticks=0;
                	state=1;
                }
                
                 }
              break;
           
              
            default:
               break;
          }
        }

    

