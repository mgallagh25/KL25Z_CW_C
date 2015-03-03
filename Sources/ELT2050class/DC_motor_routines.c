/*

This program is intended to be used with the duel DC motor controlled cars.

init_opto_sensors()   //initialize the opto sensors
sense1()  //sets the global variable opt1: 1 on tape, 0 off.  channel is the ADC channel
sense2()  //sets the global variable opt2: 1 on tape, 0 off.  channel is the ADC channel
init_accel_mma8451(void);  //initialize the onboard accelerometer
accel_mma8451(char i);		//return x, y or z acceleration data


init__DC_motors()  // activate the servo motors to accomplish the drive routines below
drive_forward(char, char)
drive_reverse(char, char)

 */





#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"
#include "my_i2c.c"    //for accelerometer
//**********************************************************/
/**************Defines Here************************/
//**********************************************************/
 
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
void init_accel_mma8451(void);  //initialize the onboard accelerometer
char accel_mma8451(char i);		//return x, y or z acceleration data



void init_DC_motors(void);   //initialize the PWM timer 0, channel 0 and 1 to operate servo motors 
void drive_forward(int, int);
void drive_reverse(int, int);

void SetNextState(void);    //main state machine
//**********************************************************/
/**************Global Variables Here************************/
//**********************************************************/
char state_opt1, state_opt2;   //these are needed for the state machines





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

    init_opto_sensors();
    init_accel_mma8451();
    init_pbs();
    init_leds();
    init_DC_motors();   
//**********************************************************/        
//*************The infinte loop.  Could use while(1);	
//**********************************************************/
    for(;;) {
		
    	drive_forward(10000, 8000);
    	
		
		// sense1();      //get updated value for state_opt1         
		// sense2();      //get updated value for state_opt2
		 
		// SetNextState();       //set to a new state depending on the state_opt variables
		sys_ticks=0;			 	
		while(sys_ticks<1000);
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
/**********************       init_DC_motors()    ******************
//**********************************************************
Initialze the timer 0 peripheral to be a PWM for channels 0 and 1.  The
PWM period will be 20ms.

These PWM outputs will be on the 12 pin header J10

            PTE30    |  |XX|  PTC1      I1_0 motor 1    
              E29    |  |XX|    C2    	I1_1 motor 1
              E23    |  |XX|    B3   	I2_0 motor 2 
              E22    |  |XX|    B2   	I2_1 motor 2 
              E21    |  |  |    B1         
              E20    |  |  |    B0          

Connections on DC motor controller:
Seek a controller that can accomodate 3V signals  The l293 will not

*/     
void init_DC_motors(void){

	init_PWM_0_0(20000);   	//I1_0
	init_PWM_0_1(20000);	//I1_1
	init_PWM_2_0(20000);	//I2_0
	init_PWM_2_1(20000);	//I2_1
				
}
/**********************************************************/
/**********************       drive_forward()    ******************/
/**********************************************************/
/*		  PTE30    |  |XX|  PTC1   	I1_0 motor 1   PWM outputs   
            E29    |  |--|    C2   	I1_1 motor 1   from these
            E23    |  |XX|    B3   	I2_0 motor 2 
            E22    |  |--|    B2   	I2_1 motor 2 
            E21    |  |  |    B1         
            E20    |  |  |    B0       */
void drive_forward(int x, int y){

PWM_0_0_pw(x);
PWM_0_1_pw(0);
PWM_2_0_pw(0);
PWM_2_1_pw(y);
}
/**********************************************************/
/************************   drive reverse() ********************/
/**********************************************************/
/*		  PTE30    |  |--|  PTC1   	I1_0 motor 1   PWM outputs   
            E29    |  |XX|    C2   	I1_1 motor 1   from these
            E23    |  |--|    B3   	I2_0 motor 2 
            E22    |  |XX|    B2   	I2_1 motor 2 
            E21    |  |  |    B1         
            E20    |  |  |    B0       */
void drive_reverse(int x, int y){

	PWM_0_0_pw(0);
	PWM_0_1_pw(x);
	PWM_2_0_pw(y);
	PWM_2_1_pw(0);
	}
/**********************************************************/
/**********************************************************/

/****************************************************************************/
/******************  void init_accel_mma8451(void)   ************************/
/****************************************************************************/
/*   This initializes the onboard accelerometer.
 * 
 * Initialize the IC0 bus first then send appropriate bits to make the sensor
 * active.
 *See the MMA8451 manual for specfic details.  
 
 The use of this function requires that you #include "my_i2c.c" in the header
 
 To use the onboard accelerometer the next two functions:
 
 char accel_mma8451(char i)  where i will be an argument that will
 determine whether you return x, y or z data
 
 i=1 	x data
 i=2  	y data
 i=3	z data
 
 The data will be 8 bit with +/- 2g sensitivity with an offset of 128.
 
 Ex values 
 	 	 	 acc=-2g	0			accelerating down at -1g
 	 	 	 acc=-1g	64	0r 0x40		tilted one side
 	 	 	 acc=0g		128 or 0x80		sitting flat
 	 	 	 acc=1g		192 or 0xc0		tilting the other side
 	 	 	 acc=2g		255	or 0xff		accelerating upwards 1g
 
 * */

void init_accel_mma8451(void){
	init_I2C0();   
	//*************Init the 8451  ***************

	I2C0WriteRegister(id_mma8451, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).  This is 2g range, no high pass data filter
	I2C0WriteRegister(id_mma8451, 0x2A, 0x01); // Writes 0x01 to general config register 1 (0x2A).  fast conversion rate, active mode
}

char accel_mma8451(char i){
	char j;
	if (i==1){
		j= I2C0ReadRegister(id_mma8451, 0x01);
		return (j+0x80)&~0x100;}
	if (i==2){
			j= I2C0ReadRegister(id_mma8451, 0x03);
			return (j+0x80)&~0x100;}
	if (i==3){
			j= I2C0ReadRegister(id_mma8451, 0x05);
			return (j+0x80)&~0x100;}
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
