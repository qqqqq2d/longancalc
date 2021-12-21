#include <curses.h>
#include <iostream>
#include <cstring>

enum class init_result
{
	success,
	error
};

init_result init()
{
	initscr();

	cbreak(); // The cbreak routine disables line buffering and erase/kill character-processing
	noecho(); // noecho routines control whether characters typed by the user are echoed
	keypad(stdscr, TRUE); // If enabled (bf is TRUE), the user can press a function key (such as an arrow key)
	mouseinterval(0); // Use mouseinterval(0) to disable click resolution

	// all mouse events
	const mmask_t mouse_support = mousemask(
		ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	if (!mouse_support)
		return init_result::error;

	printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

	return init_result::success;
}



void rectangle(int y1, int x1, int y2, int x2)
{
	mvhline(y1, x1, 0, x2 - x1);
	mvhline(y2, x1, 0, x2 - x1);
	mvvline(y1, x1, 0, y2 - y1);
	mvvline(y1, x2, 0, y2 - y1);
	mvaddch(y1, x1, ACS_ULCORNER);
	mvaddch(y2, x1, ACS_LLCORNER);
	mvaddch(y1, x2, ACS_URCORNER);
	mvaddch(y2, x2, ACS_LRCORNER);
}

void draw_grid(int y1, int x1, int y2, int x2, int columns, int rows)
{
	rectangle(y1, x1, y2, x2);
	auto col_width = (x2 - x1)/columns;
	for (int i = 0; i < columns-1; ++i)
	{
		mvvline(y1 + 1, x1+col_width*(i+1), 0, y2 - y1 - 1);
	}

	auto row_height = (y2 - y1)/rows;
	for (int i = 0; i < rows-1; ++i)
	{
		mvhline(y1 + row_height*(i+1), x1+1, 0, x2 - x1-1);
	}

	//ACS_PLUS
}

void rectangle_around_window(int height, int width, int starty, int startx)
{
	rectangle(starty - 1, startx - 1, starty + height + 1, startx + width + 1);
}

void rectangle_around_window(int height, int width, int starty, int startx, const char* title)
{
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	rectangle(starty - 1, startx - 1, starty + height + 1, startx + width + 1);
	mvprintw(starty - 1, startx + (width - static_cast<int>(strlen(title))) / 2, title);
	attroff(COLOR_PAIR(1));
}

enum class button {
	LEFT = 0x2,
	RIGHT = 0x800,
};
//WINDOW* keyb_win;
constexpr int rows = 8; // set display to four rows
constexpr int cols = 5; // set display to three columns

constexpr int keyb_grid_width = 90;
constexpr int keyb_grid_height = 48;
constexpr int keyb_starty = 11;

constexpr auto col_w = (keyb_grid_width/cols);
constexpr auto row_h = (keyb_grid_height/rows);

constexpr char keys[rows][cols] = {
	{'h','M','N','Q','P'},
	{'g','E','F','G','H'},
	{'f','I','J','K','L'},
	{'e','M','(',')','C'},
	{'d','7','8','9','*'},
	{'c','4','5','6','-'},
	{'b','1','2','3','+'},
	{'a','0','.','S','='}
};

void fill_grid()
{

	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows ; ++j)
		{
			mvprintw(j*row_h+keyb_starty, i*col_w+col_w/2, "%d %d", i, j);
			mvprintw(j*row_h+keyb_starty+row_h/2, i*col_w+col_w/2, "%c", keys[j][i]);
		}
	}
	refresh();
}

int main()
{


	auto init_r = init();
	int height = 6;
	int width = 20;
	int starty = 15;
	int startx = 49;

	draw_grid(keyb_starty,0,keyb_starty+keyb_grid_height,keyb_grid_width, cols,rows);
//	keyb_win = newwin(keyb_grid_height, keyb_grid_width, 10, 0);
	fill_grid();
	constexpr int debug_win_height = 8;
	constexpr int debug_win_width = 40;
	WINDOW* debug_win = newwin(debug_win_height, debug_win_width, 1, startx);
	rectangle_around_window(debug_win_height, debug_win_width, 1, startx, "DEBUG");
	refresh();
	scrollok(debug_win, TRUE);
	mvwprintw(debug_win, 0, 0, "Ver:%s", __VERSION__);
	wrefresh(debug_win);

	std::string mouse_str;
	if(init_r == init_result::success)
		mouse_str = "mouse success";
	else
		mouse_str = "mouse error";

	mvwprintw(debug_win, 1, 0, mouse_str.c_str());
	wrefresh(debug_win);

	while(true)
	{
		MEVENT event;
		int ch = getch();
		if (ch == KEY_MOUSE)
		{
			while (getmouse(&event) != ERR)
			{
				mvwprintw(debug_win, 3, 0, "y:%d, x:%d   ", event.y, event.x);
				mvwprintw(debug_win, 4, 0, "bstate:0x%08lx dec:%d                 ", event.bstate, event.bstate);
				if(event.bstate == (unsigned)button::LEFT)
				{
					mvwprintw(debug_win, 5, 0, "LEFT");
					auto y =(event.y-keyb_starty)/row_h;
					if(y>=rows) y=rows-1;
					auto x = event.x/col_w;
					if(x>=cols) x = cols-1;
					mvwprintw(debug_win, 6, 0, "y:%d, x:%d %c", y, x, keys[y][x]);
				}
				else if(event.bstate == (unsigned)button::RIGHT)
					mvwprintw(debug_win, 5, 0, "RIGHT");
				else
					mvwprintw(debug_win, 5, 0, "     ");
				wrefresh(debug_win);
			}
		}
		else if (ch != ERR)
		{
			if (ch == 'q')
			{
				break;
			}
		}
	}

	printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
	endwin();
	return 0;
}

