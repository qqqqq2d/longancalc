#include "computer.h"
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
#include "button.h"
#include <cmath>

char computer::get_key()
{
	char key;
	int ch = getch();
	key = 0;
	if (ch == KEY_MOUSE)
	{
		while (getmouse(&event_) != ERR)
		{
			int y = 0;
			int x = 0;

			if (event_.bstate == (unsigned)button::LEFT)
			{
				y = (event_.y - keyb_starty) / row_h;
				if (y >= rows) y = rows - 1;
				x = event_.x / col_w;
				if (x >= cols) x = cols - 1;
				key = keys[y][x];
			}
			mouse_debug_info(x, y);
		}
	}
	else if (ch != ERR)
	{
		key = ch;
	}
	return key;
}
void computer::mouse_debug_info(int x, int y)
{
	mvwprintw(debug_win_, 3, 0, "y:%d, x:%d   ", event_.y, event_.x);
	mvwprintw(debug_win_, 4, 0, "bstate:0x%08lx dec:%d                 ", event_.bstate, event_.bstate);
	if (event_.bstate == (unsigned)button::LEFT)
	{
		mvwprintw(debug_win_, 5, 0, "LEFT");
		mvwprintw(debug_win_, 6, 0, "y:%d, x:%d %c", y, x, keys[y][x]);
	}
	else if (event_.bstate == (unsigned)button::RIGHT)
		mvwprintw(debug_win_, 5, 0, "RIGHT");
	else
		mvwprintw(debug_win_, 5, 0, "     ");
	wrefresh(debug_win_);
}
computer::computer(WINDOW* debugWin, MEVENT& event) : debug_win_(debugWin), event_(event)
{
}

