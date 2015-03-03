/*


Under mux alt2
						orient with touch sensor at top
pte1  	spi1_mosi      	J2	10 
pte2 	spi1_sck		J9	4
pte3 	spi1_miso		J9	3
pte4 	spi1_pcs0(SS)	J9	2

SS25VF02 Macronix chip

1 CE	8 Vdd
2 MISO	7 Hold
3 WP*	6 SCK
4 Vss	5 MOSI

* tie WP high so it is not write protected

Some commands

c7  clear all
03  00 00 00  << three bytes specifying address    read memory
06  write enable
02  00 00 00  << three bytes specifying address  then followed by data to write 
9f  read identifier   C2 5E 16
05  read status register
20  00 00 00 sector  erase, specify sector  0-1023
d8  00 00 00 block erase , specify block    0-63


Blocks 0-63   ex block 23 = 23 x 2^16 - 24 x 2^16 - 1
Each block 16 sectors, 2^12 bytes big 

sectors 0-1023    each one 4096 bytes    
ex sector 39  addresses are 39x4096 to 40x4096-1  or in hex  0x27*0x1000 to 0x28*0x1000

4meg of memory runs from 0 to 0x40 00 00




  */

#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 


/* Global Variables */
char x;
/* Prototypes */
void SPI_Init(void);
void SPI_Send_byte(char);
char SPI_Receive_byte(void);

/**************Global Variables Here************************/

char y=0xab;
/**********************************************************/

int main(void) {
 
	//*******Local Variable Decalrations for main *****************
	int u16Counter;	
	char array[10];
	
	char x;
	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************
	syst_tick_init();
	  
	  SPI_Init();                           /* SPI Module Init */
	  
          x=0x9f;
	  while(1)
	  {
	 
	/***********************************************************************/
	/************* Push button to complete this task: Read chip id    ****/   
	    
	        
	         gpo_e(4,0);  // SPI_SS=ENABLE;
		     SPI_Send_byte(x);   //read chip id instruction
		     
	       for(u16Counter=0;u16Counter<2;u16Counter++){
	    	
		     array[u16Counter]=SPI_Receive_byte();
		     
	       }
	    
	       gpo_e(4,1);  //  SPI_SS=DISABLE;
	/************************************************************************/

	  }
	
	return 0;
    }

void SPI_Init(void)
{
    
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

  
    SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
    SIM_SCGC4|=SIM_SCGC4_SPI1_MASK;    //If using another UART, modify here
    PORTE_PCR(1)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  MOSI  See table 10.3
    PORTE_PCR(2)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  SCK  See table 10.3
    PORTE_PCR(3)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  MISO  See table 10.3
	
	//try setting the pullup registers on the PCR ports....
	
    //PORTE_PCR(4)=PORT_PCR_MUX(2);		//>> SPI  SS  See table 10.3  standard IO pin
	init_gpio_e(4,1);    //Slave select pin		       
	gpo_e(4,1);          //set high (inactive)   
    
	SPI1_BR = 0x16;              // 24MHz bus >> 93KHz SPI clock    		
	SPI1_C2 = 0x00;     
	SPI1_C1 = 0x50;  //SPI1C1 = 0b01010000  turn on spi, master mode, 
    
}

void SPI_Send_byte(char u8Data)
{
  
	while(!(SPI1_S & SPI_S_SPTEF_MASK));
	(void)SPI1_S;
	SPI1_D=u8Data;
	while(!(SPI1_S & SPI_S_SPRF_MASK));
	(void)SPI1_D;
	
}

char SPI_Receive_byte(void)
{
	SPI1_D=0x00;
	while(!(SPI1_S & SPI_S_SPRF_MASK));	
		
	return(SPI1_D);
}
