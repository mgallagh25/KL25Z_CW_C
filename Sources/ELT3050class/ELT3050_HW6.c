/*
Create code that will turn on an led if the analog value from the pot is 
greater than 1.5v and turn it off if it is less.  It will display the 
value being read from the ADC on the LCD.

 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "my_io_apps.c" /* include peripheral declarations */
#include "my_interrupts.c" /* include peripheral declarations */
#include "mcg.h" /* using XTAL and PLL does not work returns errors*/
#include "my_lcd.c" 

/**************Global Variables Here************************/

int x=0,y=0;
char state, numbuff[16];

void LCD_numbyte_out(char);     
void LCD_numint_out(int);  
void LCD_mem_out(int loc);
void SetNextState(void);



int main(void) {
 
//*******Local Variable Decalrations for main *****************
	
	
//******Initial Chip Settings******************

//  The student user should not make changes here unless
//explicitly instructed to do so
          int pll_err=pll_init(8000000,0,1,4,24,1);     //Use the crystal oscillator to drive the PLL

               


//******User chip initializations***************************************
//
// The user should initialize any modules and pins for intended use.

          syst_tick_init();
          sprintf(numbuff,"0123456789ABCDEF");
          //Initialize the lcd
              LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
          
          //initialize an led
               init_leds();
          //initialize pb
               init_pbs();
               
  
//*************************************************************** 
	 

               state=1;
	   	   
	for(;;) {	 
		 SetNextState();
	}
	
	return 0;
    }

void LCD_numbyte_out(char ch){
     char tmp;
     tmp=ch;
     tmp=tmp>>4;
     LCD_write_char(numbuff[tmp]);
     ch=ch&0x0f;
     LCD_write_char(numbuff[ch]);
     return;
}


     
void LCD_numint_out(int ch){
     int tmp;
     
     tmp=ch;
     tmp=tmp>>24;
     LCD_numbyte_out(tmp);
     tmp=ch;
     tmp=tmp>>16;
     LCD_numbyte_out(tmp);
     tmp=ch;
     tmp=tmp>>8;
     LCD_numbyte_out(tmp);
     tmp=ch;
     tmp=tmp&0x00ff;
     LCD_numbyte_out(tmp);
     return;
     
}

  
void LCD_mem_out(int loc){
     int temp;                        
     temp = *(int *)loc;      //prints out the location = value
     LCD_numint_out(loc);
     LCD_write_char('=');
     LCD_numint_out(temp);
      return;
     
}


void SetNextState(void)
 {
   static char flag=0;
       switch( state ){			      
           case 1: // idle state 1
              {
          //********Do what the state does*********    
            	  if(flag==0){
            		  LCD_write_char('\f');   //clear the screen
            		  LCD_write_string("State 1"); 
            		  flag=1;
            	  }
            	  
          //***************exit criteria**************
            	  if(!(gpi_a(1))){
            		  state=2;
            		  flag=0;
            	  }
            	  
               }
            break;
         

           case 2: // state 2
              {
    //********Do what the state does*********    
            	if(flag==0){
          		  LCD_write_char('\f');   //clear the screen
            		LCD_write_string("State 2"); 
           		  flag=1;
              	  }
            	            	  
   //***************exit criteria**************
            	  if(!(gpi_a(2))){
           		  state=3;
           		  flag=0;
            	  }              
               }
            break;

           case 3: // state 3
              {
                  //********Do what the state does*********    
                    	  if(flag==0){
                    		  LCD_write_char('\f');   //clear the screen
                    		  LCD_write_string("State 3"); 
                    		  flag=1;
                    	  }
                    	  
                  //***************exit criteria**************
                    	  if(!(gpi_a(4))){
                    		  state=1;
                    		  flag=0;
                    	  }
              
               }
            break;
            
          default:
             break;
        }
      }

  

