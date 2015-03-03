/*

This program exercises the Element14 Xtrinsic sensor board 
and the onboard accelerometer on the FRDM board

Part no.   IIC address
mma8451		0x1D				//on board accelerometer
mpl3115  	0x60    			//xtrinsic pressure and temp          
mma8491		0x55				//xtrinsic sensor accelerometer
mag3110		0x0e				//xtrinsic magnetometer

The program simply sets up each sensor then reads on value from each.

************  Pin connections  ***************
************************************************

Onboard accelerometer connected to I2C0
I2C0_SCL PTE24
I2C0_SDA PTE25

Interrupt pins  (not used)
INT1_ACCEL PTA14
INT2_ACCEL PTA15

Xtrinsic sensor board connected to I2C1

I2C0_SCL PTE0
I2C0_SDA PTE1

GPIO A 13  <<>> EN able pin: used to set mma8491 into active mode

************************************************
************  functions  ***********************
************************************************

Functions defined in my_iic.c  

init_I2C0();    //pins e24 e25 for on-board sensor   
init_I2C1();   // pins e0 e1 for xtrinsic sensors

I2C0,1WriteRegister(id_mma8451, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).  This is 2g range, no high pass data filter
I2C0,1ReadRegister(id_mma8451, 0x01);

*****************************************************
************  MMA8451 General sensor descriptions******
******************************************************

id mma8451 =	0x1D	//on board accelerometer

- Initialize sensor and put in active mode by writing to registers 0xe and 0x2a
	- set sensitivity, high-pass filter, refresh rate
- Then read data from the registers
00		status
01		msb x
02		lsb x
03		msb y
04		lsb y
05		msb z
06		lsb z

data in Acc1_x

*****************************************************
************  MMA8491 Accelerometer descriptions******
******************************************************

id mma8491 =	0x55	//xtrinsic sensor accelerometer

- can only read from this sensor
- put in active mode by setting enable EN pin
- data re measured on EN rising edge.
- allow some time before taking data, settup time

Data in Acc2_x
*****************************************************
************  MPL3115 Pressure and Temp descriptions******
******************************************************
mpl3115  0x60    //xtrinsic pressure and temp

- Initialize by
      setting to altimter mode with 0xb8 into register 0x26 
      eanble data flags by writing 0x07 into 0x13
      set the enable pin and poll (no interrupt) 0xb9 into 0x26
- Read some data from the data registers.  Ex get the MSBs for the pressure in register 0x01.  See data sheet

*****************************************************
************  MAG3110 Magnetomter descriptions******
******************************************************
mag3110	0x0e	//xtrinsic magnetometer

- Initialize by putting into active mode
         0x01 into register 0x10
- Read the data from the data registers		 
  See documentation
  
Status       R 0x00 0x01 0000 0000 Data ready status per axis
OUT_X_MSB(2) R 0x01 0x02 (0x03) data Bits [15:8] of X measurement
OUT_X_LSB(2) R 0x02 0x03 data Bits [7:0] of X measurement
OUT_Y_MSB(2) R 0x03 0x04 (0x05) data Bits [15:8] of Y measurement
OUT_Y_LSB(2) R 0x04 0x05 data Bits [7:0] of Y measurement
OUT_Z_MSB(2) R 0x05 0x06 (0x07) data Bits [15:8] of Z measurement
OUT_Z_LSB(2) R 0x06 0x07

 */





#include "derivative.h" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/

#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c"
#include "my_i2c.c"

/**************Defines Here************************/
/*
**  WHO_AM_I Device ID Register
*/
#define id_mpl3115  0x60    //xtrinsic pressure and temp          
#define id_mma8451	0x1D	//on board accelerometer
#define id_mma8491	0x55	//xtrinsic sensor accelerometer
#define id_mag3110	0x0e	//xtrinsic magnetometer

/******* Prototype Function declarations ******/



/**************Global Variables Here************************/
/*These data represent raw data.  See the sensor users manual 
to translate into recognizable values.*/ 

char acc_1_x,acc_2_x;
int mag_x,press_abs,temp_;




int main(void) {
	
	char i,j,k;
 
//*******Local Variable Declarations for main *****************

//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
     pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL
//******User chip module initializations***************************************
    
    syst_tick_init();
     
  // Convert the specific pins to the GPIO module. 
  
    init_I2C0();   
    init_I2C1();   
//*************Init the 8451  ***************
   
    I2C0WriteRegister(id_mma8451, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).  This is 2g range, no high pass data filter
    I2C0WriteRegister(id_mma8451, 0x2A, 0x01); // Writes 0x01 to general config register 1 (0x2A).  fast conversion rate, active mode

//*************Init the 8491  ***************
    
    init_gpio_a(13,1);    //accelerometer has an able pin  active mode high.
    gpo_a(13,1);
   
//*************Init the 3110  ***************

    I2C1WriteRegister(id_mag3110, 0x10, 0x01); // Writes 0x01 to address 0x10 and puts into active mode 

//*************Init the 3110  ***************

    I2C1WriteRegister(id_mpl3115, 0x26, 0xb8);
    I2C1WriteRegister(id_mpl3115, 0x13, 0x07);
    I2C1WriteRegister(id_mpl3115, 0x26, 0xb9);
    
    for(;;) {
//*****   Get date from onboard accelerometer  *****    	
    	i=I2C0ReadRegister(id_mma8451, 0x01);
		acc_1_x=(i+0x80)&~0x100;
//*****   Get date from xtrinsic board accelerometer  *****    	
     	gpo_a(13,1);     //Enable pin
    	delay_ms(100);
        i=I2C1ReadRegister(id_mma8491, 0x01);
        acc_2_x=(i+0x80)&~0x100;
        gpo_a(13,0);
        delay_ms(100);
//*****   Get date from xtrinsic board magnetometer  *****        
        i=I2C1ReadRegister(id_mag3110, 1);
        j=I2C1ReadRegister(id_mag3110, 2);
        mag_x=(((i<<8|j)+0x8000)&~0x10000);
//*****   Get date from xtrinsic board pressure and temp  *****        
        
        i=I2C1ReadRegister(id_mpl3115, 1);
        j=I2C1ReadRegister(id_mpl3115, 2);
        k=I2C1ReadRegister(id_mpl3115, 3);
        press_abs=(i<<12)|(j<<4)|(k>>4);
        i=I2C1ReadRegister(id_mpl3115, 4);
        j=I2C1ReadRegister(id_mpl3115, 5);
        temp_=(i<<4)|(j>>4);
        		
	
        
    }
	
	return 0;
    }



