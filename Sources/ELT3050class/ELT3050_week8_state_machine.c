/*

Th
 */





#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"

//**********************************************************/
/**************Defines Here************************/
//**********************************************************/
 
//**********************************************************/
/******* Prototype Function declarations ******/
//**********************************************************/
v
void SetNextState(void);    //main state machine
//**********************************************************/
/**************Global Variables Here************************/
//**********************************************************/

int main(void) {
//**********************************************************/
//*******Local Variable Declarations for main *****************
//**********************************************************/


//**********************************************************/
//******Initial Chip Settings******************
/**********************************************************/

//  The student user should not make changes here unless
//explicitly instructed to do so
     pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL
/**********************************************************/
//******User chip module initializations***************************************
/**********************************************************/

    
    syst_tick_init();

    init_pbs();
    init_leds();

//**********************************************************/        
//*************The infinte loop.  Could use while(1);	
//**********************************************************/
    for(;;) {
	
		// SetNextState();       //set to a new state depending on the state_opt variables
		sys_ticks=0;			 	
		while(sys_ticks<5);
	}
	
	return 0;
    }


/****************************************************************************/
/******************   SetNextState(void   ************************/
/****************************************************************************/
/*  This is the main state machine*/

void SetNextState(void)
{

	static char state=0;

	    
      switch( state ){	 //Switch specifies which variable controls the "case" 
          case 0: //   idle
             {
             /*    Do what the state does    */	
            	leds(state); 
                             	                              
             /*    Criteria to leave the state    */	
               if(!(gpi_a(1)))state=1;
             }
           break;

          case 1: //    straight
             {
                 /*    Do what the state does    */	
                 leds(state);
            	 
                 /*    Criteria to leave the state    */	
             
            	 if((state_opt1==0)&&(state_opt2==1))state=2;
            	 else if((state_opt1==1)&&(state_opt2==0))state=3;
            	 else if((state_opt1==1)&&(state_opt2==1))state=4;
              }
           break;

          case 2: //    right 
             {
            	 /*    Do what the state does    */	
            	 leds(state);
            	            	 
            	 /*    Criteria to leave the state    */	
            	              
            	 if((state_opt1==0)&&(state_opt2==0))state=1;
            	 else if((state_opt1==1)&&(state_opt2==1))state=4;
              }
           break;
          case 3: //
             {
            	 /*    Do what the state does    */	
            	 leds(state);
            	            	 
            	 /*    Criteria to leave the state    */	
            	              
            	 if((state_opt1==0)&&(state_opt2==0))state=1;
            	 else if((state_opt1==1)&&(state_opt2==1))state=4;
              }
           break;
          case 4: //
             {
                 /*    Do what the state does    */	
                	leds(state); 
                                 	                              
                 /*    Criteria to leave the state    */	
                   if(!(gpi_a(2)))state=0;
             }
           break;
          
         default:
            break;
       }
     }
