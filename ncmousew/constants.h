#ifndef CA096F79_7057_4924_BEA5_7F94DB412F4A
#define CA096F79_7057_4924_BEA5_7F94DB412F4A
#include <string_view>

constexpr int rows = 8; // set display to four rows
constexpr int cols = 5; // set display to three columns

constexpr int keyb_grid_width = 90;
constexpr int keyb_grid_height = 48;
constexpr int keyb_starty = 11;
constexpr int debug_win_height = 8;
constexpr int debug_win_width = 40;
constexpr int debug_win_startx = 49;


template <size_t N>
constexpr std::string_view sv(const char (&literal)[N]) {
	return std::string_view(literal, N - 1);
}

template <size_t... N>
constexpr std::array<std::string_view, sizeof...(N)> sva(
	const char (&... literals)[N]) {
	return {{sv(literals)...}};
}

constexpr auto key_names =
	sva("h", "M", "N", "Q","H",
		"g", "E", "F", "G", "CLEAR",
		"f", "I", "J", "K", "<",
		"e", "W", "(",")",":",
		"d", "7", "8", "9", "*",
		"X^3", "4", "5", "6", "-",
		"X^2", "1", "2", "3", "+",
		"SQRT", "0", ".", "S", "=");

constexpr char keys[rows][cols] = {
	{'h','M','N','Q','H'},
	{'g','E','F','G','L'},
	{'f','I','J','K','C'},
	{'e','W','(',')','/'},
	{'d','7','8','9','*'},
	{'c','4','5','6','-'},
	{'b','1','2','3','+'},
	{'a','0','.','S','='}
};

constexpr auto keys_count = key_names.size();
constexpr auto col_w = (keyb_grid_width/cols);
constexpr auto row_h = (keyb_grid_height/rows);

constexpr int calc_win_height = 5;
constexpr int calc_win_width = 20;
constexpr std::string_view allowed =".0123456789+-*/=CLqabc";


#endif /* CA096F79_7057_4924_BEA5_7F94DB412F4A */
