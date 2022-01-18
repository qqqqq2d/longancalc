#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "print_calc.h"
#include "init_result.h"

struct computer
{

	WINDOW* debug_win_;
	WINDOW* calc_win_;
	MEVENT & event_;
	init_result result_;

	computer(WINDOW* debugWin, WINDOW* calc_win, MEVENT& event, const init_result result);
	char get_key();
	void mouse_debug_info(int x, int y);
	void fill_keyboard_grid();
	void debug_key(char key, bool found);
	void debug_mem_write(double mem_var);
	void show_stored(int index, double mem_var);
	void show_mem_read(int index, double mem_var);
	void show_unary_result(char* operation, double result);
	void show_result(double result);
};

#endif //_COMPUTER_H_
