/*

This program exercises the accelerometer on the FRDM board

PN   Freescale MMA8451Q

I2C0_SCL PTE24
I2C0_SDA PTE25

INT1_ACCEL PTA14
INT2_ACCEL PTA15


400kHz  max iic clock

registers
00		status
01		msb x
02		lsb x
03		msb y
04		lsb y
05		msb z
06		lsb z




 */





#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"
#include "my_i2c.c"

/**************Defines Here************************/
#define acc_address   0x1d   //Address of the accelerometer
/*
**  WHO_AM_I Device ID Register
*/
#define WHO_AM_I_REG          0x1D
#define SYSMOD                0x0B

#define MMA8451Q_ID           0x1A
/******* Prototype Function declarations ******/



/**************Global Variables Here************************/
char    data[10];
char error=0;
int x,y,z;


int main(void) {
	
	int temp;
 
//*******Local Variable Declarations for main *****************

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
     pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL
//******User chip module initializations***************************************
    
    syst_tick_init();
     
  // Convert the specific pins to the GPIO module. 
       

      
   // init_iic0();   //initialize the iic0
    init_I2C0();   
//*************The infinte loop.  Could use while(1);	
    I2C0WriteRegister(0x1D, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).
    I2C0WriteRegister(0x1D, 0x2A, 0x39); // Writes 0x39 to general config register 1 (0x2A)
    
    for(;;) {
    	delay_ms(2000);
    	I2C0ReadMultiRegisters(0x1D, 0x01, 6, data); // Reads 6 sequential registers starting from MSB of X axis (0x01). "data" is a 6-element array of char.
    	temp=0;
    	temp=data[0];
    	temp=temp<<8;
    	temp|=data[1];
    	x=temp;

    	temp=0;
    	temp=data[2];
    	temp=temp<<8;
    	temp|=data[3];
    	y=temp;

    	temp=0;
    	temp=data[4];
    	temp=temp<<8;
    	temp|=data[5];
    	z=temp;

    }
	
	return 0;
    }



