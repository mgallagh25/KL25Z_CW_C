/*

Write to KL25Z  FRDM from Cheetah.  Pins to connect

      Cheetah
GND            2
SS             9        
SCKL           7
MISO           5
MOSI           8
 __ __ 
| 9|10|
|__|__|
| 7| 8|
|__|__|
| 5| 6|
|__|__|
| 3| 4|
|__|__|
| 1| 2|
|__|__|_______ red ribbon  looking down the holes



FRDM connections

Under mux alt2

						orient with touch sensor at top
pte1  	spi1_mosi      	J2	10 
pte2 	spi1_sck		J9	4
pte3 	spi1_miso		J9	3
pte4 	spi1_pcs0(SS)	J9	2


- Adjust baud rate to 100k on both devices
- send data from Cheetah in the format  '*'=0x2A reg  data   
 this will write data in the slave register reg where reg is a 
 number between 0-9 and the data isa single byte.  Ex, to write the number 6
 to the register 3  send  2A 03 06 




 */

#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_interrupts.c" 


/* Global Variables */
char x;
char SPI_input;
char SPI_reg[10];
/* Prototypes */
void SPI_Init(void);
void SPI_Send_byte(char);
char SPI_Receive_byte(void);

/**************Global Variables Here************************/

char y=0xab;
/**********************************************************/

int main(void) {
 
	//*******Local Variable Decalrations for main *****************


	//******Initial Chip Settings******************

	//  The student user should not make changes here unless
	//explicitly instructed to do so
	int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

	//******User chip initializations***************************************
	syst_tick_init();
	  
	  SPI_Init();                           /* SPI Module Init */
		
         
	  while(1){
	 

	  }
	
	return 0;
    }

/*****************************************************************
 * SPIInit: Initialization of the SPI.
 * It will configure and enable the SPI as a Slave, with the 
 * Receive Full Interrupt, Phase 0 and the baud rate
 *
 * Parameters: None
 *
 * Return : None
 *******************************************************************/


void SPI_Init(void)
{
    
	//SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

  
    SIM_SCGC5|=SIM_SCGC5_PORTE_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
    SIM_SCGC4|=SIM_SCGC4_SPI1_MASK;    //If using another SPI, modify here
    PORTE_PCR(1)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  MOSI  See table 10.3
    PORTE_PCR(2)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  SCK  See table 10.3
    PORTE_PCR(3)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  MISO  See table 10.3
    PORTE_PCR(4)=PORT_PCR_MUX(2)|PORT_PCR_PE_MASK;		//>> SPI  SS  See table 10.3
    	
	//try setting the pullup registers on the PCR ports....
	
 
	SPI1_BR = 0x16;              // 24MHz bus >> 93KHz SPI clock    		
	
	
	SPI1_C1 = 0b10000000; /*
	            ||||||||_____________________ LSB First (Shifter Direction)
	            |||||||_____________________ Slave Select Output Enable
	            ||||||_____________________ Clock Phase
	            |||||_____________________ Clock Polarity
	            ||||_____________________ Master/Slave Mode Select
	            |||_____________________ SPI Transmit Interrupt Enable
	            ||_____________________ SPI System Enable
	            |_____________________ SPI Interrupt Enable (for SPRF and MODF)       */
	            
	SPI1_C2 = 0b00001000;                     /*
	               || ||_____________________ SPI Pin Control 0
	               || |_____________________ SPI Stop in Wait Mode
	               || 
	               ||_____________________ Bidirectional Mode Output Enable
	               |_____________________ Master Mode-Fault Function Enable
	                                          */
	             
	                

	    
	  //  SPIBR=0x42;     //   x1/40
	    
	    SPI1_C1|=0x40;  //enable SPI
	    
	    enable_irq (INT_SPI1-16);
		
	    return;

	/****************************************************************************/

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




/********* Interrupt Functions ***********************************/
/*******************************************************************
 * SPIIsr: SPI Receiver Full Interrupt Service routine.
 * It will receive the last byte of the message sent by the MASTER
 *
 * Parameters: None
 *
 * Return : None
 *******************************************************************/

void SPI1_IRQHandler(void)
{
	unsigned char data,reg;
	
	SPI1_S;
	data=SPI1_D;       //clear flag and record data recieved 
	SPI1_C1&=~0x80;   //turn off interrupt and poll for the rest of the information 
	
	
	  //   *****   '*'  denotes a write operation   ***********
	if(data==0x2a){
	
	  while(!(SPI1_S&0x80));     //data wait for next 
	  reg=SPI1_D;       //clear flag and record register to write to
	 
	  
	  while(!(SPI1_S&0x80));     //data wait for next 
	  data=SPI1_D;       //clear flag and record register tio write to
	
	  if(reg<10){
	  
	 	 SPI_reg[reg]=data;
           	}
	 	} 
	//   *****   '#'  denotes a read operation   ***********
		
	 	else if(data=='#'){            //0x23
	    
	    while(!(SPI1_S&0x80));     //data wait for next 
	    reg=SPI1_D;       //
	 
	    SPI1_D=SPI_reg[reg];
	    while(!(SPI1_S&0x80));     //data wait for next 

	   
	 	} 
	
	
	  SPI1_S;		/* Read the SPI Status and Control Register */
	  SPI_input = SPI1_D;  /* Read of SPDR to clear interrupt */
	  SPI1_C1|=0x80;   //turn on the interrupt 

}
/******************************************************************/

