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
#include "interface.h"
#include "computer.h"
#include "init_result.h"
#include <cmath>



init_result init_screen_mouse_keyb()
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


char buf[42];

char a_buf[20];
char b_buf[20];

int r_index;
int buf_index = 0;

double a;
double b;
double result;

double memory[4];

WINDOW* debug_win;
MEVENT event;

bool filter_keys(char key);
bool char_array_contains(char key);

constexpr bool string_view_contains(const std::string_view str, const char op)
{
	return str.find(op) != std::string::npos;
	//return str.contains(op);//-std=c++2b
}


int main()
{
	bool result_calculated = false;
	const auto init_r = init_screen_mouse_keyb();

	debug_win = newwin(debug_win_height, debug_win_width, 1, debug_win_startx);
	computer c(debug_win, event);
	interface ui(c);
	

	WINDOW* calc_win = newwin(calc_win_height, calc_win_width, 1, 5);

	scrollok(debug_win, TRUE);

	mvwprintw(debug_win, 0, 0, "GCC version:%s", __VERSION__);
	wrefresh(debug_win);

	const auto mouse_status = (init_r == init_result::success) ? "mouse success" : "mouse error";
	mvwprintw(debug_win, 1, 0, mouse_status);
	wrefresh(debug_win);

	int cur_row = 0;
	char key;
	while (true)
	{
		key = ui.get_key();
		if (key == 'q')
		{
			break;
		}
		if (key == 0) continue;
		mvwprintw(debug_win, 7, 0, "key:%c, 0x%x", key, key);
		wrefresh(debug_win);

		auto found = filter_keys(key);
		mvwprintw(debug_win, 2, 0, "found:%s ", found ? "yes" : "no");
		wrefresh(debug_win);
		if (!found)
			continue;
		char fun;
		if (string_view_contains(binary_op, key) || string_view_contains(unary_op, key))
		{
			fun = key;
			strcpy(a_buf, buf);
			r_index = 0;
			cur_row = 1;
		}
		if(string_view_contains(mem_write, key))
		{
			mvwprintw(debug_win, 8, 0, "MEM WRITE");
			wrefresh(debug_win);
			if (result_calculated)
			{
				if (key == 'g')
				{
					memory[0] = result;
					mvwprintw(calc_win, 3, 0, "1:%.10g\n", result);
				}
				if (key == 'E')
				{
					memory[1] = result;
					mvwprintw(calc_win, 3, 10, "2:%.10g\n", result);
				}
				if (key == 'F')
				{
					memory[2] = result;
					mvwprintw(calc_win, 4, 0, "3:%.10g\n", result);
				}
				if (key == 'G')
				{
					memory[3] = result;
					mvwprintw(calc_win, 4, 10, "4:%.10g\n", result);
				}
			}

			wrefresh(calc_win);
			mvwprintw(debug_win, 0, 0, "MEM:%.10g\n", result);
			wrefresh(debug_win);
			continue;
		}
		if(string_view_contains(mem_read, key))
		{
			mvwprintw(debug_win, 8, 0, "MEM READ");
			wrefresh(debug_win);

			if (key == 'h')
				a = memory[0];
			if (key == 'M')
				a = memory[1];
			if (key == 'N')
				a = memory[2];
			if (key == 'Q')
				a = memory[3];
			mvwprintw(debug_win, 0, 0, "a:%.10g\n", a);
			wrefresh(debug_win);

			sprintf(a_buf, "%.10g", a);
			strcpy(buf, a_buf);
			r_index = strlen(a_buf);
			buf_index = r_index;
			mvwprintw(calc_win, 0, 0, "%s", buf);
			wrefresh(calc_win);

			continue;
		}

		if (string_view_contains(unary_op, key))
		{
			mvwprintw(debug_win, 8, 0, "UNARY");
			wrefresh(debug_win);

			char* ptr;
			a = strtod(a_buf, &ptr);
			if (key == 'a')
			{
				result = std::sqrt(a);
				mvwprintw(calc_win, 1, 0, "SQRT(%s)", a_buf);
			}
			if (key == 'b')
			{
				result = a * a;
				mvwprintw(calc_win, 1, 0, "%s^2", a_buf);
			}
			if (key == 'c')
			{
				result = a * a * a;
				mvwprintw(calc_win, 1, 0, "%s^3", a_buf);
			}
			auto count = calc_win_width - r_index;
			for (size_t i = 0; i < 20; i++)
			{
				mvwaddch(calc_win, 0, i, ' ');
			}

			mvwprintw(calc_win, 2, 0, "%.10g\n", result);
			r_index = 0;
			wrefresh(calc_win);
			continue;
		}
		if (key == '=' || key == '\n')
		{
			auto pos = strlen(a_buf);
			strcpy(b_buf, buf + pos + 1);
			//printf("buf:%s len:%d b_buf:%s\n", buf, pos, b_buf);
			char* ptr;
			a = strtod(a_buf, &ptr);
			b = strtod(b_buf, &ptr);

			mvwprintw(debug_win, 8, 0, "BINARY");
			wrefresh(debug_win);

			if (fun == '+')
			{
				result = a + b;
			}
			else if (fun == '*')
			{
				result = a * b;
			}
			else if (fun == '-')
			{
				result = a - b;
			}
			else if (fun == '/')
			{
				result = a / b;
			}
			auto count = calc_win_width - r_index;
			for (size_t i = 0; i < count; i++)
			{
				mvwaddch(calc_win, 0, r_index + i, ' ');
			}

			mvwprintw(calc_win, 2, 0, "%.10g\n", result);
			result_calculated = true;
			r_index = 0;
			wrefresh(calc_win);
			continue;
		}

		if (r_index >= 18)
			continue;

		if ((key == 'C' || key == 127 || key == 0x7) && (r_index > 0))
		{
			r_index--;
			buf_index--;
			mvwaddch(calc_win, 0, r_index, ' ');
			wmove(calc_win, 0, r_index);
			//buf[r_index] = '\0';
			buf[buf_index] = '\0';
			wrefresh(calc_win);
			continue;
		}
		else if ((key == 'L'))//CLEAR
		{
			r_index = 0;
			buf_index = 0;
			mvwprintw(calc_win, 0, 0, "                   ");
			mvwprintw(calc_win, 1, 0, "                   ");
			mvwprintw(calc_win, 2, 0, "                   ");
			wmove(calc_win, 0, r_index);
			wrefresh(calc_win);
			cur_row = 0;
			result_calculated = false;
			continue;
		}
		else
		{
			buf[buf_index] = key;
			mvwaddch(calc_win, cur_row, r_index, key);
			++r_index;
			++buf_index;
			buf[buf_index] = '\0';
		}

		mvwprintw(debug_win, 9, 0, "r_index:%d, buf_index:%d, buf:%s, a:%s, b:%s", r_index, buf_index, buf, a_buf, b_buf);
		wrefresh(debug_win);
		wrefresh(calc_win);
	}

	printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
	endwin();
	return 0;
}

bool filter_keys(char key)
{
	return (string_view_contains(binary_op, key)) || (string_view_contains(unary_op, key))
		   || (string_view_contains(spec_keys, key)) || (string_view_contains(numbers, key))
		|| (string_view_contains(mem_write, key)) || (string_view_contains(mem_read, key));
}



void compile_time_tests()
{
	static constexpr auto bin_op = '+';
	constexpr auto result = binary_op.find(bin_op) != std::string::npos || unary_op.find(bin_op) != std::string::npos;
	static_assert(result, "+ found");
	constexpr auto r_bin_op = string_view_contains(binary_op, '+');
	static_assert(r_bin_op, "+ r_bin_op found");

	constexpr char un_op = 'a';
	constexpr auto un_result = binary_op.find(un_op) != std::string::npos || unary_op.find(un_op) != std::string::npos;
	static_assert(un_result, "a found");

	constexpr char non_op = 'A';
	constexpr auto non_result =
		binary_op.find(non_op) != std::string::npos || unary_op.find(non_op) != std::string::npos;
	static_assert(!non_result, "A not found");
}

/*
	print_calc p(calc_win);
	p.add("katse1");
	p.add("katse2");
	p.add("katse3");
	p.add("katse4");
	int ch = getch();
	p.add("katse5");
	ch = getch();
	p.add("katse6");
	ch = getch();
	p.add("katse7");
	ch = getch();
*/