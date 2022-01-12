#ifndef _COMPUTER_H_
#define _COMPUTER_H_
#include <curses.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include "ncfunctions.h"
#include "constants.h"
#include "calc_colors.h"
#include "print_calc.h"
#include <cmath>

struct computer
{

	WINDOW* debug_win_;
	MEVENT & event_;

	computer(WINDOW* debugWin, MEVENT& event);
	char get_key();
	void mouse_debug_info(int x, int y);
};

#endif //_COMPUTER_H_
