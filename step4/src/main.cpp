#include <Arduino.h>       
#include "uart.h"

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

// the setup function runs once when you press reset or power the board
void setup() {
//    pinMode(PB4, OUTPUT);
//    digitalWrite(PB4, HIGH);

    pinMode(PB5, INPUT_PULLUP);
    digitalWrite(PB5, HIGH);    //turn on the pullup

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, HIGH);

    
    for(char r = 0; r < rows; r++)
    {
         pinMode(rowPins[r], INPUT_PULLUP);    //set the row pins as input
         digitalWrite(rowPins[r], HIGH);    //turn on the pullups
    }
   
   for(char c = 0; c < cols; c++)
   {
         pinMode(colPins[c], OUTPUT);   //set the column pins as output
   }

   delay(2000);
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

//      delay(3000);
//      digitalWrite(PB4, LOW);
//      digitalWrite(LED_GREEN, LOW);
//      delay(2000);
//      digitalWrite(PB4, HIGH);
//      digitalWrite(LED_GREEN, HIGH);

    char key = getKey();
    _put_char(key);
    if(digitalRead(PB5) == LOW)
    {
        digitalWrite(LED_GREEN, LOW);
        //_put_char('W');
    }
}
