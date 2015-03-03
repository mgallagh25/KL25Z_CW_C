/*

This program is intended to be used with the duel servo motor controlled cars.

init_opto_sensors()   //initialize the opto sensors
sense1()  //sets the global variable opt1: 1 on tape, 0 off.  channel is the ADC channel
sense2()  //sets the global variable opt2: 1 on tape, 0 off.  channel is the ADC channel
init_accel_mma8451(void);  //initialize the onboard accelerometer
accel_mma8451(char i);		//return x, y or z acceleration data


init__servo_motors()  // activate the servo motors to accomplish the drive routines below
drive_left()
drive_right()
drive_straight()



 */





#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"
#include "my_i2c.c"    //for accelerometer
//**********************************************************/
/**************Defines Here************************/
//**********************************************************/
#define motor_nuetral 1500  //    approx 0 speed for the motors
#define max1 2000     //max in one direction for the motor attached to timer channel 1
#define max0 1000     // max in the other direction for the motor attached to timer channel 0
 
#define opt_max1 2500     // max value used in the state machine opt sense
#define opt_min1 1000     // min value used in the state machine opt sense

#define opt_max2 2500     // max value used in the state machine opt sense
#define opt_min2 1000     // min value used in the state machine opt sense

#define id_mma8451	0x1D	//on board accelerometer
//**********************************************************/
/******* Prototype Function declarations ******/
//**********************************************************/
void init_opto_sensors(void);   //initialize the opto sensors
void sense1(void);         //sets the global variable opt1: 1 on tape, 0 off.  channel is the ADC channel
void sense2(void);         //sets the global variable opt2: 1 on tape, 0 off.  channel is the ADC channel



void init_servo_motors(void);   //initialize the PWM timer 0, channel 0 and 1 to operate servo motors 
void drive_left(void);   //initialize the PWM timer 0, channel 0 and 1 to operate servo motors 
void drive_right(void);   //initialize the PWM timer 0, channel 0 and 1 to operate servo motors 
void drive_straight(void);   //initialize the PWM timer 0, channel 0 and 1 to operate servo motors 

void SetNextState(void);    //main state machine
//**********************************************************/
/**************Global Variables Here************************/
//**********************************************************/
char state_opt1, state_opt2, state;




int main(void) {
//**********************************************************/
//*******Local Variable Declarations for main *****************
//**********************************************************/

	
//**********************************************************/
//******Initial Chip Settings******************
//**********************************************************/

//  The student user should not make changes here unless
//explicitly instructed to do so
     pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL
//******User chip module initializations***************************************
    
 
    init_opto_sensors();
   
//**********************************************************/        
//*************The infinte loop.  Could use while(1);	
//**********************************************************/
    for(;;) {
	
    	


    	//drive_straight();
	
		
		
		 sense1();      //get updated value for state_opt1         
		 sense2();      //get updated value for state_opt2
		 
		// SetNextState();       //set to a new state depending on the state_opt variables
		 					 	
		
	}
	
	return 0;
    }

//**********************************************************/
/************** init_opto_sensors()    *********************/
//**********************************************************/
/*    Plan on the subsequent code for the sensors to be attached to E30  B0 
  
   
 
   ADC0_23  PTE30    |xx|  |  PTC1    ADC0_15
        4B    E29    |  |  |    C2         11
        7A    E23    |  |  |    B3         13
        3     E22    |  |  |    B2         12
        4A    E21    |  |  |    B1         9
        0     E20    |  |xx|    B0         8 
                     -------
 */

void init_opto_sensors(void){
     init_adc0();   
}
//**********************************************************/
/*************  void sense1(void) ********************
//**********************************************************
This routine will be used to determine if the optical sensor is
"seeing" anything like tape. You will need to add your own values 
to the max/min variables.  It uses the A to D converter to take in an
analog signal from the sensor.  Because seeing can be different for 
different materials and different configurations of the sensor may be
"on" depending on your setup.  Adjust max / min so that the sensor really
is seeing something "off" and something "on".

- The state of the sensor is given by the global variable state_opt1 = 1 or 0

- You must declare  char state_opt1=0 in the global variables section

- It has built in hysteresis.

- You must initialize the init_sensors in the initialization section.  

void sense1(void) will set a global state variable state_opt1 or 2
depending on the ADC value. 


Use the pin below for this routine
>>>ADC0_23  PTE30    |XX|  |  PTC1    ADC0_15
        4B    E29    |  |  |    C2         11
        7A    E23    |  |  |    B3         13
        3     E22    |  |  |    B2         12
        4A    E21    |  |  |    B1         9
        0     E20    |  |  |    B0         8 

*/ 
void sense1(void){

      int x=adc0_value(23);
   
      switch( state_opt1 ){			      //Switch specifies which variable controls the "case" 
          case 0: // State 0  on/off optical marker
             {
             if(x>opt_max1)state_opt1=1;
             }
           break;
        
          case 1: // state 1 off/on optical marker
             {
             if(x<opt_min1)state_opt1=0;
             } 
            
           break;
           
         default:
            break;
       }
     }
     
/* Use the pin below for this routine
   ADC0_23  PTE30    |  |  |  PTC1    ADC0_15
        4B    E29    |  |  |    C2         11
        7A    E23    |  |  |    B3         13
        3     E22    |  |  |    B2         12
        4A    E21    |  |  |    B1         9
>>>     0     E20    |  |XX|    B0         8 

*/ 
void sense2(void){

      int x=adc0_value(8);
   
      switch( state_opt2 ){			      //Switch specifies which variable controls the "case" 
          		  
      	  case 0: // State 0  on/off optical marker
             {
             if(x>opt_max2)state_opt2=1;
             }
           break;
        
          case 1: // state 1 off/on optical marker
             {
             if(x<opt_min2)state_opt2=0;
             } 
            
           break;
           
         default:
            break;
       }
     }

//**********************************************************/
/**********************       init_servo_motors()    ******************
//**********************************************************
Initialze the timer 0 peripheral to be a PWM for channels 0 and 1.  The
PWM period will be 20ms.

These PWM outputs will be on the 12 pin header J10

            PTE30    |  |XX|  PTC1    
              E29    |  |XX|    C2    
              E23    |  |  |    B3    
              E22    |  |  |    B2    
              E21    |  |  |    B1         
              E20    |  |  |    B0          

Connections on the servos:

White or yellow   C1 or C2      signal wire
red              vcc or 5V      power
black            gnd            ground


*/     
void init_servo_motors(void){

	init_PWM_0_0(20000);
	init_PWM_0_1(20000);
		
}

/**********************       drive_left()    ******************/
void drive_left(void){
	
	PWM_0_0_pw(max0);
	PWM_0_1_pw(0);
}
/**********************       drive_right()    ******************/
void drive_right(void){
	
	PWM_0_0_pw(0);
	PWM_0_1_pw(max1);
}
/**********************       drive_straight()    ****************/
void drive_straight(void){
	
	PWM_0_0_pw(max0);
	PWM_0_1_pw(max1);
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
