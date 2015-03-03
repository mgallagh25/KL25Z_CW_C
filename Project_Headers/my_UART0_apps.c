/*  

This is the comment header for the my_UART0_apps.c file in the header folder.

This comment header describes how the functions in both the master and slave device work.  There are functions 
which should be used in the slave device and others in the master.

Master

void init_serial_master(void)
void serial_tx(char x, char y)    send the value y to the xth element in UART0_buffer[10]
char serial_rx(char x)  send back to the master the value in the xth element of UART0_buffer[10]

Slave

void init_serial_slave(void)    set the IRQ priority to 3 (lower priority than sys tick
void UART0_IRQHandler(void)    does not need to be invoked in the main program.  handles the commands
                               coming from the master as an interrupt.

The pin assignments for all the UART0:

A1      Rx      Alt2
A2      Tx      Alt2
A14     Rx      Alt3
A15     Tx      Alt3
B16     Rx      Alt3
B17     Tx      Alt3
D6      Rx      Alt3   <<use these
D7      Tx      Alt3   <<


These programs talk to each other through the UART0_ comm port.  
There is an array in the slave device that simply is managed by the master device with the two functions

value serial_rx(i);      //returns the char value of the ith array element given in the argument
serial_tx(i, value); //writes the char value in the ith array element in the slave

The slave device has an interrupt function that waits for the master to tell it where and what to to
with the 10 element array UART0_buffer[10]. 

*******************  Functions  *****************************

              Master functions

void init_serial_master(void)    set baud rate, normal UART0_ operation, 8 bits ,no parity
char serial__rx(char i);      //returns the char value of the ith array element given in the argument
void serial_tx(char i, char value); //writes the char value in the ith array element in the slave

              Slave functions
void init_serial_slave    set baud rate, normal UART0_ operation, 8 bits ,no parity, turn on interrupt with 
                   lower priority than the sys_tick
void UART0_IRQHandler(void)
   
       Has two main part: if a # is first sent by the master it tells the slave the master
       wants to READ a value in the UART0__reg[]
       
       If a * is sent it wants the slave to WRITE a value to a specific register.     
********************  Variable  ************************************************
 
  
char UART0_buffer[10] slave device only global array to store communiation data

*****************************************************************************
 If you want to use the UART0_ with a hyperterminal

Turn on UART0_


set up hyperterm

9600 baud
com1  if using com1
flow control  none
parity none

Pin assignments on male serial
1
x x x x x    
 x x x x
       9

2: computer rx, MPU tx
3: MPU rx, computer tx
5: gnd

Max232 pin assignments. The max232 chip brings up the voltage on the line from ttl to -12v >>> +12v

1:3   0.1 uf cap  between pins 1:3 place a capacitor
4:5   0.1 uf cap
15    gnd
16    vcc = 5V

mpu>>computer

11    tx from mpu(pin11)
14    tx to computer   

computer>>mpu
13    rx from computer
12    rx to mpu(pin12)  

*******************************************************************************************/



volatile char UART0_buffer[12];


 
void init_serial_slave(void){
	
/******************************************************
 *              system settings                      */	

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_UART0SRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); 
  
    SIM_SCGC5|=SIM_SCGC5_PORTD_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
    SIM_SCGC4|=SIM_SCGC4_UART0_MASK;    //If using another UART, modify here
    PORTD_PCR(6)=PORT_PCR_MUX(3);		//>> RX  Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
    PORTD_PCR(7)=PORT_PCR_MUX(3);		//>> Tx  Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
    		       


/******************************************************
 *              peripheral settings                      */	
		
	
  UART0_BDH=0x4;   	//divide by 1 bit takes 
  UART0_BDL=0x00;		//(1/24Mz x 2 x 1024)s 13 bits 0b0,0100,0000,0000
  UART0_C1=0 ;     //two-wire/normal 8bit/no parity
  UART0_C2=0x24;   //polling RX and TX/   TIE TCIE RIE ILIE TE RE RWU SBK
                   //                      0   0    1    0    0  1  0   0   
  
  enable_irq (INT_UART0-16);  
  set_irq_priority (INT_UART0-16, 3);   //otherwise the sys tick interrupt can't happen
  
  return;
}

/*                    UART0_IRQHandler
 * 
*************************************************************************
 * 
 * */
void UART0_IRQHandler(void){
	
	char i,j;
UART0_S1=0xff;
i=UART0_S1; // to clear read once

i=UART0_D; 


if(i=='#'){     //    Master sent an rx command so send the master back the value UART0_buffer[i]

//wait for the adress of the register wanted
UART0_C2=0x04;  //turn on reciever only, no interrupt    TIE TCIE RIE ILIE TE RE RWU SBK

sys_ticks=0;
while((!(UART0_S1 & UART0_S1_RDRF_MASK)) && (sys_ticks<100));  //wait for RDRF to go high TDRE TC RDRF IDLE OR NF FE PF

i= UART0_D;

//transmit on, send the register specified

UART0_C2=0x08;      //Turn on transmit   TIE TCIE RIE ILIE TE RE RWU SBK
UART0_D=UART0_buffer[i];       
while((!(UART0_S1 & UART0_S1_TC_MASK)) && (sys_ticks<100));  //wait for TC to go high TDRE TC RDRF IDLE OR NF FE PF
    
            
}

     
else if(i=='*'){        // Master sent a tx command so the slave should write the value to UART0_buffer[i]        



//wait for the adress of the register wanted
UART0_C2=0x04;  //turn on reciever only, no interrupt    TIE TCIE RIE ILIE TE RE RWU SBK

sys_ticks=0;
while((!(UART0_S1 & UART0_S1_RDRF_MASK)) && (sys_ticks<100));  //wait for RDRF to go high TDRE TC RDRF IDLE OR NF FE PF

i= UART0_D;

//wait for the data 
UART0_C2=0x04;  //turn on reciever only, no interrupt    TIE TCIE RIE ILIE TE RE RWU SBK
sys_ticks=0;
while((!(UART0_S1 & UART0_S1_RDRF_MASK)) && (sys_ticks<100));  //wait for RDRF to go high TDRE TC RDRF IDLE OR NF FE PF
j= UART0_D;

UART0_buffer[i]=j;
     
} 


else;

UART0_C2=0x24;
i=UART0_S1;

return;
}  
/*******************************************************************************************/
void init_serial_master(void){

	
/******************************************************
 *              system settings                      */	
	

	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	SIM_SOPT2 &= ~(SIM_SOPT2_UART0SRC_MASK);
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); 
  
    SIM_SCGC5|=SIM_SCGC5_PORTD_MASK;	//>> Use new pin value here (Port value A,B,C,D or E)
    SIM_SCGC4|=SIM_SCGC4_UART0_MASK;    //If using another UART, modify here
    PORTD_PCR(6)=PORT_PCR_MUX(3);		//>> RX  Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
    PORTD_PCR(7)=PORT_PCR_MUX(3);		//>> Tx  Use pin n and mux value here.  Its mux 3 usually, sometimes 4.  See table 10.3
    		       


/******************************************************
 *              peripheral settings                      */	
		
	
  UART0_BDH=0x4;   	//divide by 1 bit takes 
  UART0_BDL=0x00;		//(1/24Mz x 2 x 1024)s 13 bits 0b0,0100,0000,0000
  UART0_C1=0 ;     //two-wire/normal 8bit/no parity
  UART0_C2=0;      //polling RX and TX/   TIE TCIE RIE ILIE TE RE RWU SBK
                   //                      0   0    0    0    0  0  0   0   
  return;
}


char serial_rx(char i){
     char j;

     //turn on transmit and tell the slave the command is "read" with a #
     
     
     UART0_C2=0x08;      //Turn on transmit   TIE TCIE RIE ILIE TE RE RWU SBK
     UART0_D='#';       
     sys_ticks=0;
     while((!(UART0_S1 & UART0_S1_TC_MASK)) && (sys_ticks<100));  //wait for TC to go high TDRE TC RDRF IDLE OR NF FE PF
     
     
     UART0_D=i;           //send the address you want       
     sys_ticks=0;
     while((!(UART0_S1 & UART0_S1_TC_MASK)) && (sys_ticks<100));  //wait for TC to go high TDRE TC RDRF IDLE OR NF FE PF
     
     
     //turn on receive and wait for the data to come back
     
       
     UART0_C2=0x04;  //turn on reciever
     while((!(UART0_S1 & 0x20))&&(sys_ticks<100));      
     j= UART0_D;
     

     return j;
     }



void serial_tx(char i,char j){

     //turn on transmit and tell the slave the command is "write" with a *
     
     sys_ticks=0;
     UART0_C2=0x08;      //Turn on transmit   TIE TCIE RIE ILIE TE RE RWU SBK
     UART0_D='*';       
     while((!(UART0_S1 & UART0_S1_TC_MASK)) && (sys_ticks<100));  //wait for TC to go high TDRE TC RDRF IDLE OR NF FE PF
     UART0_D=i;       

     sys_ticks=0;
     while((!(UART0_S1 & UART0_S1_TC_MASK)) && (sys_ticks<100));  //wait for TC to go high TDRE TC RDRF IDLE OR NF FE PF
     UART0_D=j;       

     sys_ticks=0;
     while((!(UART0_S1 & UART0_S1_TC_MASK)) && (sys_ticks<100));  //wait for TC to go high TDRE TC RDRF IDLE OR NF FE PF

     return;
}
