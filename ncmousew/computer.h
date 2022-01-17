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
	void debug_mem_write();
	void show_stored(int index, double mem_var);
};

#endif //_COMPUTER_H_
