#include <Arduino.h>       
#include "uart.h"
extern "C" {
#include "lcd/lcd.h"
#include <stdio.h>
#include <stdint.h>
}

// Keypad Scanning without Library
// ee-diary.blogspot.com

const int rows = 4; // set display to four rows
const int cols = 4; // set display to three columns

const char keys[rows][cols] = {
              {'1','2','3','A'},
               {'4','5','6','B'},
               {'7','8','9','C'},
               {'*','0','#','D'}
               };
               
int rowPins[rows] = {PA4, PA5, PA6, PA7};
int colPins[cols] = {PB0, PB1, PB10, PB11};

static void init_uart0(void)
{
   // enable GPIO clock 
   rcu_periph_clock_enable(RCU_GPIOA);
   // enable USART0 clock 
   rcu_periph_clock_enable(RCU_USART0);  
   // configure USART0
   usart_deinit(USART0);
   usart_baudrate_set(USART0, 115200U);
   usart_word_length_set(USART0, USART_WL_8BIT);
   usart_stop_bit_set(USART0, USART_STB_1BIT);
   usart_parity_config(USART0, USART_PM_NONE);
   usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
   usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
   usart_receive_config(USART0, USART_RECEIVE_ENABLE);
   usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
   usart_enable(USART0);
}



// the setup function runs once when you press reset or power the board
void setup() {
    // enable GPIO clock 
   init_uart0();
    char buf[64];
  // initialize LCD and clear the display to black 
  Lcd_Init();
  LCD_Clear(BLACK);

  _put_char('T');
  _put_char('E');
  _put_char('R');
  _put_char('E');
  sprintf(buf, "ver:%s", __VERSION__);
  LCD_ShowString(0, 0, (u8 const *) buf, GBLUE);
  BACK_COLOR = BLACK;

    pinMode(PA4, INPUT_PULLUP);
    digitalWrite(PA4, HIGH);    //turn on the pullups 
    
    for(char r = 0; r < rows; r++)
    {
         pinMode(rowPins[r], INPUT_PULLUP);    //set the row pins as input
         digitalWrite(rowPins[r], HIGH);    //turn on the pullups
    }
   
   for(char c = 0; c < cols; c++)
   {
         pinMode(colPins[c], OUTPUT);   //set the column pins as output
   }
}


char getKey(){
      char k = 0;
      
      for(char c = 0; c < cols; c++)
      {
        digitalWrite(colPins[c], LOW);
         for(char r = 0; r < rows; r++)
         {
            if(digitalRead(rowPins[r]) == LOW)
            {
                delay(200);    //ms debounce time
                while(digitalRead(rowPins[r])== LOW);
                k = keys[r][c];
            }
         }
        digitalWrite(colPins[c], HIGH); 
      }
      return k;
}

// the loop function runs over and over again forever
void loop() 
{
    char key = getKey();
    _put_char(key);
}
