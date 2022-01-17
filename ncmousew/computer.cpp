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
computer::computer(WINDOW* debugWin, WINDOW* calc_win, MEVENT& event, const init_result result) : debug_win_(debugWin), calc_win_(calc_win), event_(event), result_(result)
{
	draw_keyboard_grid(keyb_starty, 0, keyb_starty + keyb_grid_height, keyb_grid_width, cols, rows);
	fill_keyboard_grid();
	rectangle_around_window(debug_win_height, debug_win_width, 1, debug_win_startx, "DEBUG");
	rectangle_around_window(calc_win_height, calc_win_width, 1, 5, "CALC");
	refresh();
	scrollok(debug_win_, TRUE);
	mvwprintw(debug_win_, 0, 0, "GCC version:%s", __VERSION__);
	wrefresh(debug_win_);


	const auto mouse_status = (result_ == init_result::success) ? "mouse success" : "mouse error";
	mvwprintw(debug_win_, 1, 0, mouse_status);
	wrefresh(debug_win_);
}

void computer::fill_keyboard_grid()
{
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			mvprintw(j * row_h + keyb_starty, i * col_w + col_w / 2, "%d %d", i, j);
//			mvprintw(j*row_h+keyb_starty+row_h/2, i*col_w+col_w/2, "%c", keys[j][i]);
			const auto index = j * cols + i;
			auto text = key_names.at(index).data();
			mvprintw(j * row_h + keyb_starty + row_h / 2, i * col_w + col_w / 2 - strlen(text) / 2, "%s", text);
		}
	}
	refresh();
}
void computer::debug_key(char key, bool found)
{
	mvwprintw(debug_win_, 7, 0, "key:%c, 0x%x", key, key);
	mvwprintw(debug_win_, 2, 0, "found:%s ", found ? "yes" : "no");
	wrefresh(debug_win_);
}
void computer::debug_mem_write(double mem_var)
{
	mvwprintw(debug_win_, 0, 0, "MEM WR:%.10g\n", mem_var);
	wrefresh(debug_win_);
}
void computer::show_stored(int index, double mem_var)
{
	if(index == 0)
	{
		mvwprintw(calc_win_, 3, 0, "          ");
		mvwprintw(calc_win_, 3, 0, "1:%.10g", mem_var);
	}
	if(index == 1)
	{
		mvwprintw(calc_win_, 3, 10, "          ");
		mvwprintw(calc_win_, 3, 10, "2:%.10g", mem_var);
	}
	if(index == 2)
	{
		mvwprintw(calc_win_, 4, 0, "          ");
		mvwprintw(calc_win_, 4, 0, "3:%.10g", mem_var);
	}
	if(index == 3)
	{
		mvwprintw(calc_win_, 4, 10, "          ");
		mvwprintw(calc_win_, 4, 10, "4:%.10g\n", mem_var);
	}
	wrefresh(calc_win_);
}
void computer::show_mem_read(int index, double mem_var)
{
	mvwprintw(debug_win_, 0, 0, "a:%.10g\n", mem_var);
	wrefresh(debug_win_);
	mvwprintw(calc_win_, 0, 0, "                    ");
	mvwprintw(calc_win_, 0, 0, "%.10g", mem_var);
	wrefresh(calc_win_);
}
