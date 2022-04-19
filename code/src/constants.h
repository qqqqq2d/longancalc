#ifndef CA096F79_7057_4924_BEA5_7F94DB412F4A
#define CA096F79_7057_4924_BEA5_7F94DB412F4A
#include <string_view>
#include <array>

constexpr int rows = 8; // set display to four rows
constexpr int cols = 5; // set display to three columns

constexpr int keyb_grid_width = 90;
constexpr int keyb_grid_height = 48;
constexpr int keyb_starty = 13;
constexpr int debug_win_height = 11;
constexpr int debug_win_width = 60;
constexpr int debug_win_startx = 29;


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
	sva("R1", "R2", "R3", "R4","ON/OFF",
		"W1", "W2", "W3", "W4", "BAT",
		"RED", "GREEN", "BLUE", "WHITE", "CLEAR",
		"SQRT", "X^1/3", "X^2","X^Y","<",

		"7", "8", "9", "1/x", "|x|",
		"4", "5", "6", "*", ":",
		"1",  "2", "3", "+", "-",
		"+/-", "0", ".","pi", "=");

constexpr char keys[rows][cols] = {
	{'h','M','N','Q','q'},
	{'g','E','F','G','H'},
	{'f','I','J','K','L'},
	{'a','b','c','^','C'},

	{'7','8','9', 'Z','A'},
	{'4','5','6','*','/'},
	{'1','2','3','+', '-'},
	{'S','0','.','p','='}
};

constexpr auto keys_count = key_names.size();
constexpr auto col_w = (keyb_grid_width/cols);
constexpr auto row_h = (keyb_grid_height/rows);

constexpr int calc_win_height = 5;
constexpr int calc_win_width = 20;

constexpr std::string_view numbers ="0123456789";
constexpr std::string_view binary_op ="+-*/^";
constexpr std::string_view unary_op ="abcZ";
constexpr std::string_view spec_keys ="\x7f\xa\x7.=CLp";
constexpr std::string_view mem_read ="hMNQ";
constexpr std::string_view mem_write ="gEFG";
constexpr std::string_view color_keys ="fIJK";

#endif /* CA096F79_7057_4924_BEA5_7F94DB412F4A */
