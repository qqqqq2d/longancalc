#ifndef NCMOUSEW__PRINT_CALC_H
#define NCMOUSEW__PRINT_CALC_H
#include "calc_colors.h"
#include "constants.h"
#include <curses.h>
#include <vector>

struct print_calc
{
	print_calc(int rowIndex);
	print_calc(WINDOW* debugWin);
	void add(std::string_view buf);
	int row_index_ = 0;
	WINDOW* debug_win_;
	void scroll_text();
	std::vector<std::string_view> screen_buf_ = {"                    ",
												 "                    ",
												 "                    ",
												 "                    ",
												 "                    "};
};

#endif //NCMOUSEW__PRINT_CALC_H
