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

computer::computer()
{
	result_ = init_screen_mouse_keyb();
	debug_win_ = newwin(debug_win_height, debug_win_width, 1, debug_win_startx);
	calc_win_ = newwin(calc_win_height, calc_win_width, 1, 5);
	draw_keyboard_grid(keyb_starty, 0, keyb_starty + keyb_grid_height, keyb_grid_width, cols, rows);
	fill_keyboard_grid();
	rectangle_around_window(debug_win_height, debug_win_width, 1, debug_win_startx);
	rectangle_around_window(calc_win_height, calc_win_width, 1, 5);
	refresh();
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
	mvwprintw(debug_win_, 8, 0, "MEM READ");
	wrefresh(debug_win_);
	mvwprintw(debug_win_, 0, 0, "a:%.10g\n", mem_var);
	wrefresh(debug_win_);
	mvwprintw(calc_win_, 0, 0, "                    ");
	mvwprintw(calc_win_, 0, 0, "%.10g", mem_var);
	wrefresh(calc_win_);
}
void computer::show_unary_result(char* operation, double result)
{
	mvwprintw(debug_win_, 8, 0, "UNARY");
	wrefresh(debug_win_);
	mvwprintw(calc_win_, 0, 0, "                    ");
	mvwprintw(calc_win_, 1, 0, "                    ");
	mvwprintw(calc_win_, 1, 0, operation);
	mvwprintw(calc_win_, 2, 0, "%.10g\n", result);
	wrefresh(calc_win_);
}
void computer::show_result(double result)
{
	mvwprintw(debug_win_, 8, 0, "BINARY");
	wrefresh(debug_win_);
	mvwprintw(calc_win_, 2, 0, "%.10g\n", result);
	wrefresh(calc_win_);
}
void computer::back_space(int cur_row, int r_index)
{
	mvwaddch(calc_win_, cur_row, r_index, ' ');
	wmove(calc_win_, cur_row, r_index);
	wrefresh(calc_win_);
}
void computer::clear(int r_index)
{
	mvwprintw(calc_win_, 0, 0, "                   ");
	mvwprintw(calc_win_, 1, 0, "                   ");
	mvwprintw(calc_win_, 2, 0, "                   ");
	wmove(calc_win_, 0, r_index);
	wrefresh(calc_win_);
}
void computer::add_key(int cur_row, int r_index, char key)
{
	mvwaddch(calc_win_, cur_row, r_index, key);
	wrefresh(calc_win_);
}
void computer::debug_buf(char fun, int r_index, int buf_index, char* buf, char* a_buf, char* b_buf)
{
	mvwprintw(debug_win_, 9, 0, "fun:%c, r_index:%d, buf_index:%d, buf:%s, a:%s, b:%s", fun, r_index, buf_index, buf, a_buf, b_buf);
	wrefresh(debug_win_);
}

void computer::debug_handle_binary_operation(char fun, char * a_buf, char * b_buf, char * buf, int pos, double a, double b)
{
	mvwprintw(debug_win_, 10, 0,"fun:%c, a_buf:%s, b_buf:%s, buf:%s, pos:%d, a:%g, b:%g\n", fun, a_buf, b_buf, buf, pos, a, b);
}

void computer::end()
{
	printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
	endwin();
}
void computer::show_a(char* a_buf)
{
	mvwprintw(calc_win_, 0, 0, "%s", a_buf);
	wrefresh(calc_win_);
}

void computer::debug(char *buf)
{
	mvwprintw(debug_win_, 10, 0,buf);
}

init_result computer::init_screen_mouse_keyb()
{
	initscr();

	cbreak(); // The cbreak routine disables line buffering and erase/kill character-processing
	noecho(); // noecho routines control whether characters typed by the user are echoed
	keypad(stdscr, TRUE); // If enabled (bf is TRUE), the user can press a function key (such as an arrow key)
	mouseinterval(0); // Use mouseinterval(0) to disable click resolution

	// all mouse events
	const mmask_t mouse_support = mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	if (!mouse_support)
		return init_result::error;

	printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

	return init_result::success;
}

