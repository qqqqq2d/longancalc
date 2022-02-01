#include <string_view>
#include <string>
#include <Arduino.h>     
#include "uart.h"
extern "C" {
#include "lcd/lcd.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
}

#include "mcu.h"
#include "interface.h"
#include "calculator.h"


mcu c;
interface<mcu> ui(c);
calculator calc(ui);

void setup() {

}


void loop() 
{
 calc.loop_step();
    
}
