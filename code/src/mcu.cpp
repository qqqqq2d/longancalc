#include <string_view>
#include <string>
#include <Arduino.h>    
#include "mcu.h"
#include "init_result.h"
#include "uart.h"
extern "C" {
#include "lcd/lcd.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
}


char mcu::get_key()
{
    if(digitalRead(PB7) == LOW)
    {
        digitalWrite(LED_GREEN, LOW);
        printf("OFF\n");
    }

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
	  if(k == 0) return k;
	  printf("key:%c\n", k);
      return k;
}
void mcu::mouse_debug_info(int x, int y)
{

}

mcu::mcu()
{
	pinMode(PB7, INPUT_PULLUP);// ON/OFF nupp
	digitalWrite(PB7, HIGH);

	constexpr int analogPin = PA3;//patarei pinge
	pinMode(analogPin,INPUT_ANALOG);

	pinMode(LED_GREEN, OUTPUT);//kalkulaatori väljalülitamise väljund
	digitalWrite(LED_GREEN, HIGH);

	init_uart0();
	std::string_view proov = "\nlongancalc1\n";
	printf(proov.data());
	Lcd_Init();
	LCD_Clear(BLACK);
	char buf[20];
	sprintf(buf, "kalkulaator");
	LCD_ShowString(8*8, 0, (u8 const *) buf, text_color);

	for(auto r = 0; r < rows; r++)
	{
		pinMode(rowPins[r], INPUT_PULLUP);    //set the row pins as input
		digitalWrite(rowPins[r], HIGH);    //turn on the pullups
	}
		
	for(auto c = 0; c < cols; c++)
	{
		pinMode(colPins[c], OUTPUT);   //set the column pins as output
	}
}

void mcu::fill_keyboard_grid()
{

}
void mcu::debug_key(char key, bool found)
{
	printf("debug_key:%c, found:%d\n", key, found);
}
void mcu::debug_mem_write(double mem_var)
{
	printf("MEM WR:%.10g\n", mem_var);
}
void mcu::show_stored(int index, double mem_var)
{
	char temp_buf[20];		
	if(index == 0)
	{
		LCD_ShowString(0, 16*3, (u8 const *) "       ", text_color);
		sprintf(temp_buf,"1:%.5g", mem_var);
		LCD_ShowString(0, 16*3, (u8 const *) temp_buf, text_color);
	}
	if(index == 1)
	{
		LCD_ShowString(10*8, 16*3, (u8 const *) "       ", text_color);
		sprintf(temp_buf,"2:%.5g", mem_var);
		LCD_ShowString(10*8, 16*3, (u8 const *) temp_buf, text_color);
	}
	if(index == 2)
	{
		LCD_ShowString(0, 16*4, (u8 const *) "       ", text_color);
		sprintf(temp_buf,"3:%.5g", mem_var);
		LCD_ShowString(0, 16*4, (u8 const *) temp_buf, text_color);
	}
	if(index == 3)
	{
		LCD_ShowString(10*8, 16*4, (u8 const *) "       ", text_color);
		sprintf(temp_buf,"4:%.5g", mem_var);
		LCD_ShowString(10*8, 16*4, (u8 const *) temp_buf, text_color);
	}
}
void mcu::show_mem_read(int index, double mem_var)
{
	char temp_buf[20];
	sprintf(temp_buf,"%.10g", mem_var);
	LCD_ShowString(0, 0, (u8 const *) temp_buf, text_color);
}
void mcu::show_unary_result(char* operation, double result)
{
	LCD_ShowString(0, 0, (u8 const *) operation, text_color);
	printf("unary_result:%.10g\n", result);
	char temp_buf[20];
	sprintf(temp_buf,"%.10g", result);
	LCD_ShowString(0, 16, (u8 const *) temp_buf, text_color);
}
void mcu::set_color(display_color c)
{
	printf("setting color to %d\n", c);
	if(c==display_color::blue)
		text_color = lcd_colors::blue;
	if(c==display_color::red)
		text_color = lcd_colors::red;
	if(c==display_color::green)
		text_color = lcd_colors::green;
	if(c==display_color::white)
		text_color = lcd_colors::white;
}
void mcu::show_result(double result)
{
	printf("result:%.10g\n", result);
	char temp_buf[20];
	sprintf(temp_buf,"%.10g", result);
	LCD_ShowString(0, 16*2, (u8 const *) temp_buf, text_color);
}
void mcu::back_space(int cur_row, int r_index)
{
	LCD_ShowChar(r_index*8,16*cur_row,' ',0,text_color);
}
void mcu::clear(int r_index)
{
	LCD_ShowString(0, 0, (u8 const *) "                   ", text_color);
	LCD_ShowString(0, 16, (u8 const *) "                   ", text_color);
	LCD_ShowString(0, 16*2, (u8 const *) "                  ", text_color);
	//LCD_Clear(BLACK);
}
void mcu::add_key(int cur_row, int r_index, char key)
{
	LCD_ShowChar(r_index*8,16*cur_row,key,0,text_color);
	if(debugging)
		printf("add_key:%c, r_index:%d, cur_row:%d\n", key, r_index, cur_row);
}
void mcu::debug_buf(char fun, int r_index, int buf_index, char* buf, char* a_buf, char* b_buf)
{
	printf("[debug_buf] fun:%x, r_index:%d, buf_index:%d, buf:%s, a:%s, b:%s\n", fun, r_index, buf_index, buf, a_buf, b_buf);
}

void mcu::debug_handle_binary_operation(char fun, char * a_buf, char * b_buf, char * buf, int pos, double a, double b)
{
	printf("fun:%c, a_buf:%s, b_buf:%s, buf:%s, pos:%d, a:%g, b:%g\n", fun, a_buf, b_buf, buf, pos, a, b);
}

void mcu::end()
{
	
}
void mcu::show_a(char* a_buf)
{
	LCD_ShowString(0, 0, (u8 const *) a_buf, text_color);
}
void mcu::debug(char *buf)
{
	printf("debug:%s\n", buf);
}
init_result mcu::init_screen_mouse_keyb()
{
	return init_result::success;
}
