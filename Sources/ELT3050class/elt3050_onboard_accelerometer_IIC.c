/*

This program exercises the accelerometer on the FRDM board

This program will initialize the onboard accelerometer on the FRDM board and
read the 8 MSBs for each of the three axes x, y and z.

Functions:
main: 
 --Initialize iic0 on the kl25z
 
 --initialzes the MMA8451Q sensor to continuously generate x, y and z
    init_I2C0()
  -- turn on the iic0 peripheral
          
void I2C0WriteRegister(slave_id, target register, data):
  --  used to write to a device connected to the iic bus that has internal
      coontrol and data registers
      
char I2C0ReadRegister(slave_id, target register)
  -- Returns the value in the target register

Input/ output
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
#include "my_i2c.c"

/**************Defines Here************************/
#define acc_address   0x1d   //Address of the accelerometer
/*
**  WHO_AM_I Device ID Register
*/
#define slave_id          0x1D

/******* Prototype Function declarations ******/



/**************Global Variables Here************************/
char    data[10];
char error=0;
int x,y,z;


int main(void) {
	
	int temp;
	char i,j;
 
//*******Local Variable Declarations for main *****************

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
     pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL
//******User chip module initializations***************************************
    
     
  // Convert the specific pins to the GPIO module. 
  
    init_I2C0();   
//*************Leave the idle state and begin reading.
    I2C0WriteRegister(slave_id, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).  This is 2g range, no high pass data filter
    I2C0WriteRegister(slave_id, 0x2A, 0x01); // Writes 0x01 to general config register 1 (0x2A).  fast conversion rate, active mode
    
    for(;;) {
    	delay_ms(10);
    	i=I2C0ReadRegister(slave_id, 0x01);   //0x01  are the MSBs of the X data
    	        x=(i+0x80)&~0x100;           //  Add 0x80  clear the 9th bit   
        i=I2C0ReadRegister(slave_id, 0x03);     //0x03  MSB of Y data
        		y=(i+0x80)&~0x100;
        i=I2C0ReadRegister(slave_id, 0x05);
        		z=(i+0x80)&~0x100;
    }
	
	return 0;
    }



