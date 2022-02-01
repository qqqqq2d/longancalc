#include <string_view>
#include <string>
#include "print_calc.h"


void print_calc::add(std::string_view buf)
{
	if (row_index_ >= calc_win_height)
		scroll_text();
	screen_buf_[row_index_] = buf;
	mvwprintw(debug_win_, row_index_, 0, buf.data());
	wrefresh(debug_win_);
	if (row_index_ <= calc_win_height)
		row_index_++;
}
void print_calc::scroll_text()
{
	screen_buf_.erase(screen_buf_.begin());
	for (int i = 0; i < calc_win_height - 1; ++i)
	{
		std::string buf(screen_buf_.at(i));
		const auto nr_of_spaces = calc_win_width-buf.length();
		for (int j = 0; j < nr_of_spaces; ++j)
		{
			buf+=" ";
		}
		mvwprintw(debug_win_, i, 0, buf.c_str());
	}
	screen_buf_.push_back("");
	--row_index_;
}
print_calc::print_calc(WINDOW* debugWin) : debug_win_(debugWin)
{

}
