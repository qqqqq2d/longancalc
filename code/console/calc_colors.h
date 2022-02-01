#ifndef NCMOUSEW_MAIN_CPP_CALC_COLORS_H
#define NCMOUSEW_MAIN_CPP_CALC_COLORS_H
#include <curses.h>
enum class calc_colors
{
	red,
	green,
	blue,
	white
};

inline short get_color(calc_colors color)
{
	if(color==calc_colors::green)
		return COLOR_GREEN;
	if(color==calc_colors::white)
		return COLOR_WHITE;
	if(color==calc_colors::blue)
		return COLOR_BLUE;
	if(color==calc_colors::red)
		return COLOR_RED;
}

#endif //NCMOUSEW_MAIN_CPP_CALC_COLORS_H
