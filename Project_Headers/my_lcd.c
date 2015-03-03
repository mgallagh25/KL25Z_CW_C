/*
***************            LCD applications           ************

User functions

void LCD_init(char mode1, char mode2)
void LCD_pos_xy(char x, char y)
void LCD_write_char(char c)
void LCD_write_string (char *c)
void LCD_display_on(void)
void LCD_display_off(void)
void LCD_cursor_on(void)
void LCD_cursor_off(void)
void LCD_cursor_blink_on(void)
void LCD_cursor_blink_off(void)

Back end functions

void LCD_send_byte(char address, char data)
void LCD_delay_ms(char)
void LCD_send_nibble(char data)



******************   Physical Connections  ******************
 1      GND
 2      5V
 3      Adjust = GND
 4 A16  LCD_RS
 5      LCD_RW = GND
 6 A17  LCD_E
 7      GND
 8      GND
 9      GND
 10     GND
 11 B8  LCD_D1
 12 B9  LCD_D2
 13 B10 LCD_D3
 14 B11 LCD_D4
 
 
 
 */


#define LCD_SEC_LINE    0x40    // Address of the second line of the LCD

// LCD configuration constants
#define CURSOR_ON       2
#define CURSOR_OFF      0
#define CURSOR_BLINK    1
#define CURSOR_NOBLINK  0
#define DISPLAY_ON      4
#define DISPLAY_OFF     0
#define DISPLAY_8X5     0
#define DISPLAY_10X5    4
#define _2_LINES        8
#define _1_LINE         0
static char lcd_mode;	



void LCD_delay_ms (unsigned char lcd_time){
	unsigned int temp;
	for(;lcd_time;lcd_time--) for(temp=8000;temp;temp--);
}

void LCD_send_nibble(char data)
{
	int temp=0;
	temp|=data;
	temp=temp<<8;


	GPIOB_PDOR=temp;
    // pulse the LCD enable line
	gpo_a(17,1);
	for (data=240; data; data--);
	gpo_a(17,0);
}		

void LCD_init(char mode1, char mode2)
{
	char aux;
	// Configure the pins as outputs
	init_gpio_a(16,1);   //RS  
	init_gpio_a(17,1);   //Enable
	init_gpio_b(8,1);   //d1
	init_gpio_b(9,1);   //d2
	init_gpio_b(10,1);   //d3
	init_gpio_b(11,1);   //d4
			
// Set the LCD data pins to zero
	gpo_a(16,0);
	gpo_a(17,0);
	gpo_b(8,0);
	gpo_b(9,0);
	gpo_b(10,0);
	gpo_b(11,0);
	LCD_delay_ms(15);
	// LCD 4-bit mode initialization sequence
	// send three times 0x03 and then 0x02 to finish configuring the LCD
	for(aux=0;aux<3;++aux)
	{		
	  LCD_send_nibble(3);
	  LCD_delay_ms(5);
	}
	LCD_send_nibble(2);
	// Now send the LCD configuration data
	LCD_send_byte(0,0x20 | mode1);
	LCD_send_byte(0,0x08 | mode2);
	lcd_mode = 0x08 | mode2;
	LCD_send_byte(0,1);
	LCD_send_byte(0,6);
}

void LCD_send_byte(char address, char data)
{
  unsigned int temp;
	gpo_a(16,address);              // config the R/S line   0= command, 1=data
    gpo_a(17,0);                    // set LCD enable line to 0
	LCD_send_nibble(data >> 4);     // send the higher nibble
	LCD_send_nibble(data & 0x0f);   // send the lower nibble
	for (temp=12000; temp; temp--);
}


//**************************************************************************
//* LCD cursor position set
//**************************************************************************
//* Calling arguments:
//* char x : column (starting by 0)
//* char y : line (0 or 1)
//**************************************************************************
void LCD_pos_xy(char x, char y)
{
  char address;
  if (y) address = LCD_SEC_LINE; else address = 0;
  address += x;
  LCD_send_byte(0,0x80|address);
}

//**************************************************************************
//* Write a character on the display
//**************************************************************************
//* Calling arguments:
//* char c : character to be written
//**************************************************************************
//* Notes :
//* \f clear the display
//* \n and \r return the cursor to line 1 column 0
//**************************************************************************
void LCD_write_char(char c)
{
  switch (c)
	{
	  case '\f' :	
	    LCD_send_byte(0,1);
	    LCD_delay_ms(5);
	    break;
	  case '\n' :
	  case '\r' :	
	    LCD_pos_xy(0,1);
	    break;
	  default:
	    LCD_send_byte(1,c);
   }
}

//**************************************************************************
//* Write a string on the display
//**************************************************************************
//* Calling arguments:
//* char *c : pointer to the string
//**************************************************************************
void LCD_write_string (char *c)
{
	char count=0;
	while (*c)
	{
	  LCD_write_char(*c);
	  c++;
	}
}

//**************************************************************************
//* Turn the display on
//**************************************************************************
void LCD_display_on(void)
{
	lcd_mode |= 4;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the display off
//**************************************************************************
void LCD_display_off(void)
{
	lcd_mode &= 0b11111011;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the cursor on
//**************************************************************************
void LCD_cursor_on(void)
{
  lcd_mode |= 2;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the cursor off
//**************************************************************************
void LCD_cursor_off(void)
{
	lcd_mode &= 0b11111101;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn on the cursor blink function
//**************************************************************************
void LCD_cursor_blink_on(void)
{
	lcd_mode |= 1;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn off the cursor blink function
//**************************************************************************
void LCD_cursor_blink_off(void)
{
	lcd_mode &= 0b11111110;
	LCD_send_byte (0,lcd_mode);
}

