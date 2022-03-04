#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "print_calc.h"
#include "init_result.h"

struct computer
{
	WINDOW* debug_win_;
	WINDOW* calc_win_;
	MEVENT event_;
	init_result result_;
	computer();
	char get_key();
	void mouse_debug_info(int x, int y);
	void fill_keyboard_grid();
	void debug_key(char key, bool found);
	void debug_mem_write(double mem_var);
	void show_stored(int index, double mem_var);
	void show_mem_read(int index, double mem_var);
	void show_unary_result(char* operation, double result);
	void show_result(double result);
	void back_space(int cur_row, int r_index);
	void clear(int r_index);
	void add_key(int cur_row, int r_index, char key);
	void debug_buf(char fun, int r_index, int buf_index, char buf[42], char a_buf[20], char b_buf[20]);
	void end();
	void show_a(char a_buf[20]);
	void debug(char *buf);
	void debug_handle_binary_operation(char fun, char * a_buf, char * b_buf, char * buf, int pos, double a, double b);

	init_result init_screen_mouse_keyb();

};

#endif //_COMPUTER_H_
