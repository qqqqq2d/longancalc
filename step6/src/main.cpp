#include <Arduino.h>     
#include "uart.h"
extern "C" {
#include "lcd/lcd.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
}


#define CYCLES         4  // 2 yields a heartbeat effect
#define SHORT_DELAY  100  // 1/10 second
#define LONG_DELAY  100  


const int rows = 8; // set display to four rows
const int cols = 5; // set display to three columns

int analogPin = PA3;

const char keys[rows][cols] = {
              {'h','M','N','Q','P'},
               {'g','E','F','G','H'},
               {'f','I','J','K','L'},
               {'e','M','(',')','C'},
               {'d','7','8','9','*'},
               {'c','4','5','6','-'},
               {'b','1','2','3','+'},
               {'a','0','.','S','='}
               };

//int rowPins[rows] = {PC14, PA4, PA3, PB15, PA6, PA12, PB10, PA8};
//int colPins[cols] = {PC15, PA0, PB8, PA11, PB11 };

//PA8 ei sobi r7-ks
//                    r0   r1   r2   r3    r4   r5    r6    r7          
int rowPins[rows] = {PC14, PA4, PB4, PB9, PA6, PB6, PB10, PA11};

//                    c0    c1    c2   c3    c4
int colPins[cols] = {PC15, PA0, PB8, PB5, PB11 };

//ALGNE:
//int rowPins[rows] = {PA0, PA4, PA3, PA11, PA6, PB14, PB10, PB12};
//int colPins[cols] = {PC15, PC14, PB8, PB13, PB11 };

//                    r0      r1   r2   r3    r4   r5    r6    r7          
//int rowPins[rows] = {PC14, PA4, PA3, PB15, PA6, PA12, PB10, PA8};

//                    c0    c1    c2   c3    c4
//int colPins[cols] = {PC15, PA0, PB8, PA11, PB11 };

char buf[20];
char ibuf[20];

char a_buf[20];
char b_buf[20];

int r_index;

void setup() {

r_index =0 ;

  pinMode(PB7, INPUT_PULLUP);
  digitalWrite(PB7, HIGH);

  pinMode(analogPin,INPUT_ANALOG);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);

   init_uart0();
   printf("\nlongancalc\n");

  // initialize LCD and clear the display to black 
  Lcd_Init();
  LCD_Clear(BLACK);

  //sprintf(buf, "g++ ver::%s", __VERSION__);
  sprintf(buf, "1234567890123456789");
  LCD_ShowString(0, 0, (u8 const *) buf, GBLUE);
  //sprintf(buf, "                   ");
  LCD_ShowString(0, 16, (u8 const *) buf, GBLUE);
  LCD_ShowString(0, 16*2, (u8 const *) buf, GBLUE);
  LCD_ShowString(0, 16*3, (u8 const *) buf, GBLUE);
  LCD_ShowString(0, 16*4, (u8 const *) buf, GBLUE);

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

constexpr auto allowed =".0123456789+-*/=CL";

double a;
double b;
double result;


void loop() 
{
    if(digitalRead(PB7) == LOW)
    {
        digitalWrite(LED_GREEN, LOW);
        _put_char('W');
    }
    //char key = uart_rbyte();

    char key = getKey();
    _put_char(key);
    if(key == 'H')
    {
      int val = analogRead(analogPin);  // read the input pin
      printf("Analog read:%d\n", val);
    }

    // delay(2000); EI TOHI OLLA NII PIKK, kui klahve lugeda
    /*
    if(key == 0) return;
    bool found = false;
    for (size_t i = 0; i < 18; i++)
    {
      if(allowed[i]==key)
        found=true;
    }
    
    if (!found)
      return;
    
    if(key == '+')
    {
      strcpy(a_buf, buf);
      //printf("a:%s\n", a_buf);
    }

    if(key == '=')
    {
       auto pos = strlen(a_buf);
       strcpy(b_buf, buf+pos+1);
           //printf("buf:%s len:%d b_buf:%s\n", buf, pos, b_buf);
      char *ptr;
      a = strtod(a_buf, &ptr);
      b = strtod(b_buf, &ptr);
      result = a+b;
      printf("=%.10g\n", result);
      sprintf(buf, "                   ");
      r_index = 0;
      return;
    }





    if(r_index>=18)
    return;

    if((key == 'C') && (r_index>0))
    {
      char cbuf[20];
      sprintf(cbuf, "                   ");
      LCD_ShowString(0, 16, (u8 const *) cbuf, GBLUE);
      r_index--;      
      buf[r_index] = '\0';
    }
    else if((key == 'L') && (r_index>0))
    {
      sprintf(buf, "                   ");
      r_index=0;
    }
    else 
    {
      buf[r_index] = key;
      ++r_index;    
      buf[r_index] = '\0';
    }
    LCD_ShowString(0, 16, (u8 const *) buf, GBLUE);
    sprintf(ibuf, "%d", r_index);
    LCD_ShowString(0, 32, (u8 const *) ibuf, GBLUE);

    */
}
