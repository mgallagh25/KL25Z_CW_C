/*Note from developer
Hello Mauricio,

 

I am giving a software lab course at the University. As students are learning the basics of low-level programming (register level), 
I do not use Processor Expert in this course. The final experiment is accessing the accelerometer through I2C, so I have developed 
a library for this purpose, derived from an example library for Kinetis Family, which I simplified and suited for my needs. I am 
sending the .h and .c files attached to this reply.

For controlling the accelerometer, you just have to write the right values to the right config registers. Remember that the 
accelerometer address (SlaveID) is 0x1D.
If you just want to read raw (unfiltered) data from XYZ axes, using a full scale range of 2g, you may initialize the accelerometer using this code:

I2CWriteRegister(0x1D, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).
I2CWriteRegister(0x1D, 0x2A, 0x39); // Writes 0x39 to general config register 1 (0x2A)

Now the accelerometer starts working. It will make a new read (all 3 axes) every 640ms. If you want other periods, you just 
have to write the right value on 0x2A register, instead of 0x39. For 160ms, write 0x31; for 80ms, write 0x29; for 20ms, write 0x21.
You may read the latest collected values this way:

I2CReadMultiRegisters(0x1D, 0x01, 6, data); // Reads 6 sequential registers starting from MSB of X axis (0x01). "data" is a 6-element array of char.

The result will be the values of 3 axes loaded on "data" array: [MSB_X, LSB_X, MSB_Y, LSB_Y, MSB_Z, LSB_Z]. You may also test if new data is 
available before reading using:

dr = (I2CReadRegister(0x1D, STATUS) & 0x08); // dr is zero if no new data is available; it is 0x08 if new data is available.
I hope that it helps. Just message me if you need something else.
 

Antonio
*/

#include "derivative.h"

#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */
#define i2c0_DisableAck()       I2C0_C1 |= I2C_C1_TXAK_MASK
#define i2c0_EnableAck()        I2C0_C1 &= ~I2C_C1_TXAK_MASK
#define i2c0_RepeatedStart()    I2C0_C1 |= I2C_C1_RSTA_MASK
#define i2c0_EnterRxMode()      I2C0_C1 &= ~I2C_C1_TX_MASK
#define i2c0_write_byte(data)   I2C0_D = data

#define i2c0_Start()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK

#define i2c0_Stop()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK

#define i2c0_Wait()               while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
                                  I2C0_S |= I2C_S_IICIF_MASK;


void init_I2C0(void)
{
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
  PORTE_PCR24 = PORT_PCR_MUX(5);
  PORTE_PCR25 = PORT_PCR_MUX(5);
  I2C0_F  = 0x14;
  I2C0_C1 = I2C_C1_IICEN_MASK;
}

/* Inicia Transmissao I2C
 * SlaveID: endereco do slave
 * "Mode" define modo Read (1) ou Write (0)
 */
void IIC0_StartTransmission (char SlaveID, char Mode)
{
  SlaveID = SlaveID << 1;
  SlaveID |= (Mode & 0x01);
  i2c0_Start();
  i2c0_write_byte(SlaveID);
}

void Pause(void){
    int n;
    for(n=1;n<50;n++) {
      asm("nop");
    }
}

/* Le um registrador do slave I2C
 * SlaveID: endereco do slave
 * RegisterAddress: endereco do registrador interno do slave
 * Retorna o valor do registrador
 */
char I2C0ReadRegister(char SlaveID, char RegisterAddress)
{
  char result;

  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(RegisterAddress); 
  i2c0_Wait();
  i2c0_RepeatedStart();
  i2c0_write_byte((SlaveID << 1) | 0x01);
  i2c0_Wait();
  i2c0_EnterRxMode();
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  result = I2C0_D ; // Dummy read
  i2c0_Wait();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  result = I2C0_D ; // Le o byte
  Pause();
  return result;
}

/* Escreve um byte no registrador interno do slave I2C
 * SlaveID: endereco do slave
 * RegisterAddress: endereco do registrador interno do slave
 * Data: Valor a escrever
 */
void I2C0WriteRegister(char SlaveID, char RegisterAddress, char Data)
{
  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(RegisterAddress);
  i2c0_Wait();
  i2c0_write_byte(Data);
  i2c0_Wait();
  i2c0_Stop();
  Pause();
}

/* Le "N" registradores internos do slave I2C
 * SlaveID: endereco do slave
 * RegisterAddress: endereco do primeiro registrador interno do slave a ser lido
 * n: Numero de registradores a serem lidos em sequencia
 * Resultado armazenado no ponteiro *r
 */
void I2C0ReadMultiRegisters(char SlaveID, char RegisterAddress, char n, char * r)
{
  char i;

  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(RegisterAddress);
  i2c0_Wait();
  i2c0_RepeatedStart();
  i2c0_write_byte((SlaveID << 1) | 0x01);
  i2c0_Wait();
  i2c0_EnterRxMode();
  i2c0_EnableAck();
  i = I2C0_D ; // Dummy read
  i2c0_Wait();

  for(i=0;i<n-2;i++) // le n-1 bytes
  {
    *r = I2C0_D;
    r++;
    i2c0_Wait();
  }
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  *r = I2C0_D; // Le penultimo byte
  r++;
  i2c0_Wait();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  *r = I2C0_D; // Le ultimo byte
  Pause();
}
