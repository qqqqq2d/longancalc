#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_
#include "interface.h"
#include "computer.h"

#include "calculator.h"
#include "computer.h"
#include "interface.h"
#include <string>
#include <string_view>
#include <cmath>
#include <cstring>

constexpr bool string_view_contains(const std::string_view str, const char op)
{
	return str.find(op) != std::string::npos;
	//return str.contains(op);//-std=c++2b
}

bool filter_keys(char key)
{
	return (string_view_contains(binary_op, key)) || (string_view_contains(unary_op, key))
		   || (string_view_contains(spec_keys, key)) || (string_view_contains(numbers, key))
		   || (string_view_contains(mem_write, key)) || (string_view_contains(mem_read, key));
}


template<typename interface>
struct calculator
{
	int cur_row = 0;
	char key = 0;

	char buf[42];

	char a_buf[20];
	char b_buf[20];

	int r_index = 0;
	int buf_index = 0;

	double a = 0;
	double b = 0;
	double result = 0;

	double memory[4];

	bool result_calculated = false;


	interface& ui;

	calculator(interface& ui_par) : ui(ui_par)
	{
	}
	void handle_key_added(int cur_row, char key)
	{
		buf[buf_index] = key;
		ui.add_key(cur_row, r_index, key);
		++r_index;
		++buf_index;
		buf[buf_index] = '\0';
	}
	void handle_clear(bool& result_calculated, int& cur_row)
	{
		r_index = 0;
		buf_index = 0;
		ui.clear(r_index);
		cur_row = 0;
		result_calculated = false;
	}
	void handle_back_space()
	{
		r_index--;
		buf_index--;
		buf[buf_index] = '\0';
		ui.back_space(r_index);
	}
	void handle_binary_operation(char fun)
	{
		auto pos = strlen(a_buf);
		strcpy(b_buf, buf + pos + 1);
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
			result = pow(a, b);
		}
		ui.show_result(result);
		r_index = 0;
	}
	void handle_unary_operation(char key)
	{
		char operation[40];
		char* ptr;
		a = strtod(a_buf, &ptr);
		if (key == 'a')
		{
			result = sqrt(a);
			sprintf(operation, "SQRT(%s)", a_buf);
		}
		if (key == 'b')
		{
			result = a * a;
			sprintf(operation, "%s^2", a_buf);
		}
		if (key == 'c')
		{
			result = a * a * a;
			sprintf(operation, "%s^3", a_buf);
		}
		if (key == 'Z')
		{
			result = 1 / a;
			sprintf(operation, "1/%s", a_buf);
		}
		ui.show_unary_result(operation, result);
		r_index = 0;
	}
	void handle_mem_read(char key)
	{
		int i;
		for (i = 0; i < mem_read.length(); ++i)
		{
			if (mem_read.at(i) == key) break;
		}
		if (i < mem_read.length())
		{
			a = memory[i];
			sprintf(a_buf, "%.10g", a);
			strcpy(buf, a_buf);
			r_index = strlen(a_buf);
			buf_index = r_index;
			ui.show_mem_read(i, a);
		}
	}
	void handle_mem_write(bool result_calculated, char key)
	{
		ui.debug_mem_write(result);
		if (result_calculated)
		{
			int i;
			for (i = 0; i < mem_write.length(); ++i)
			{
				if (mem_write.at(i) == key) break;
			}
			if (i < mem_write.length())
			{
				memory[i] = result;
				ui.show_stored(i, result);
			}
		}
	}
	void pi()
	{
		a = std::numbers::pi;
		sprintf(a_buf, "%.13g", a);
		strcpy(buf, a_buf);
		r_index = strlen(a_buf);
		buf_index = r_index;
		ui.show_a(a_buf);
	}
	bool loop_step()
	{
		key = ui.get_key();
		if (key == 'p')//PI
		{
			pi();
			return true;
		}
		if (key == 'q')
		{
			return false;
		}
		if (key == 0) return true;
		auto found = filter_keys(key);
		ui.debug_key(key, found);

		if (!found)
			return true;
		char fun;

		if (string_view_contains(binary_op, key) || string_view_contains(unary_op, key))
		{
			fun = key;
			strcpy(a_buf, buf);
			r_index = 0;
			cur_row = 1;
		}
		if (string_view_contains(mem_write, key))
		{
			handle_mem_write(result_calculated, key);
			return true;
		}
		if (string_view_contains(mem_read, key))
		{
			handle_mem_read(key);
			return true;
		}
		if (string_view_contains(unary_op, key))
		{
			handle_unary_operation(key);
			return true;
		}
		if (key == '=' || key == '\n')
		{
			handle_binary_operation(fun);
			result_calculated = true;
			return true;
		}

		if (r_index >= 18)
			return true;

		if ((key == 'C' || key == 127 || key == 0x7) && (r_index > 0))
		{
			handle_back_space();
			return true;
		}
		else if ((key == 'L'))//CLEAR
		{
			handle_clear(result_calculated, cur_row);
			return true;
		}
		else
		{
			handle_key_added(cur_row, key);
		}
		ui.debug_buf(r_index, buf_index, buf, a_buf, b_buf);
		return true;
	}
};


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
#endif //_CALCULATOR_H_
