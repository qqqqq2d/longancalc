#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "print_calc.h"
#include "init_result.h"

struct computer
{

	WINDOW* debug_win_;
	MEVENT & event_;
	init_result result_;

	computer(WINDOW* debugWin, MEVENT& event, const init_result result);
	char get_key();
	void mouse_debug_info(int x, int y);
	void fill_keyboard_grid();
};

#endif //_COMPUTER_H_
