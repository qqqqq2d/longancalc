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
WINDOW* calc_win;

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
//	a = std::numbers::pi;
//	sprintf(a_buf, "%.15g", a);
//	printf("%s", a_buf);
//	return 0;
	bool result_calculated = false;
	const auto init_r = init_screen_mouse_keyb();

	debug_win = newwin(debug_win_height, debug_win_width, 1, debug_win_startx);
	calc_win = newwin(calc_win_height, calc_win_width, 1, 5);
	computer c(debug_win, calc_win, event, init_r);
	interface ui(c);



	int cur_row = 0;
	char key;
	while (true)
	{
		key = ui.get_key();
		if(key == 'p')//PI
		{
			a = std::numbers::pi;
			sprintf(a_buf, "%.13g", a);
			strcpy(buf, a_buf);
			r_index= strlen(a_buf);
			buf_index=r_index;
			ui.show_a(a_buf);
			continue;
		}
		if (key == 'q')
		{
			break;
		}
		if (key == 0) continue;
		auto found = filter_keys(key);
		ui.debug_key(key, key);

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
			ui.debug_mem_write(result);
			if (result_calculated)
			{
				int i;
				for (i = 0; i < mem_write.length(); ++i)
				{
					if(mem_write.at(i) == key) break;
				}
				if(i<mem_write.length())
				{
					memory[i] = result;
					ui.show_stored(i, result);
				}
			}
			continue;
		}
		if(string_view_contains(mem_read, key))
		{
			int i;
			for (i = 0; i < mem_read.length(); ++i)
			{
				if(mem_read.at(i) == key) break;
			}
			if(i<mem_read.length())
			{
				a = memory[i];
				sprintf(a_buf, "%.10g", a);
				strcpy(buf, a_buf);
				r_index = strlen(a_buf);
				buf_index = r_index;
				ui.show_mem_read(i, a);
			}
			continue;
		}

		if (string_view_contains(unary_op, key))
		{
			char operation[40];
			char* ptr;
			a = strtod(a_buf, &ptr);
			if (key == 'a')
			{
				result = std::sqrt(a);
				sprintf(operation,"SQRT(%s)", a_buf);
			}
			if (key == 'b')
			{
				result = a * a;
				sprintf(operation,"%s^2", a_buf);
			}
			if (key == 'c')
			{
				result = a * a * a;
				sprintf(operation,"%s^3", a_buf);
			}
			if (key == 'Z')
			{
				result = 1/a;
				sprintf(operation,"1/%s", a_buf);
			}
			ui.show_unary_result(operation, result);
			r_index = 0;
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
			else if (fun == 'P')
			{
				result = std::pow(a,b);
			}
			ui.show_result(result);
			result_calculated = true;
			r_index = 0;
			continue;
		}

		if (r_index >= 18)
			continue;

		if ((key == 'C' || key == 127 || key == 0x7) && (r_index > 0))
		{
			r_index--;
			buf_index--;
			buf[buf_index] = '\0';
			ui.back_space(r_index);
			continue;
		}
		else if ((key == 'L'))//CLEAR
		{
			r_index = 0;
			buf_index = 0;
			ui.clear(r_index);
			cur_row = 0;
			result_calculated = false;
			continue;
		}
		else
		{
			buf[buf_index] = key;
			ui.add_key(cur_row, r_index, key);
			++r_index;
			++buf_index;
			buf[buf_index] = '\0';
		}
		ui.debug_buf(r_index, buf_index, buf, a_buf, b_buf);
	}
	ui.end();
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