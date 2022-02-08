#ifndef F6B66ADB_11B6_4425_BD95_1890A7B31894
#define F6B66ADB_11B6_4425_BD95_1890A7B31894
#include "init_result.h"
#include "constants.h"
constexpr bool debug = false;
struct mcu
{
	mcu();
	char get_key();
	void mouse_debug_info(int x, int y);
	void fill_keyboard_grid();
	void debug_key(char key, bool found);
	void debug_mem_write(double mem_var);
	void show_stored(int index, double mem_var);
	void show_mem_read(int index, double mem_var);
	void show_unary_result(char* operation, double result);
	void show_result(double result);
	void back_space(int r_index);
	void clear(int r_index);
	void add_key(int cur_row, int r_index, char key);
	void debug_buf(int r_index, int buf_index, char buf[42], char a_buf[20], char b_buf[20]);
	void end();
	void show_a(char a_buf[20]);
	init_result init_screen_mouse_keyb();
	//                    r0   r1   r2   r3    r4   r5    r6    r7          
	int rowPins[rows] = {PC14, PA4, PB4, PB9, PA6, PB6, PB10, PA11};

	//                    c0    c1    c2   c3    c4
	int colPins[cols] = {PC15, PC13, PB8, PB5, PB11 };
};


#endif /* F6B66ADB_11B6_4425_BD95_1890A7B31894 */