#include <curses.h>
#include <cstring>

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

void draw_keyboard_grid(int y1, int x1, int y2, int x2, int columns, int rows)
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
	rectangle(starty - 1, startx - 1, starty + height, startx + width + 1);
}

void rectangle_around_window(int height, int width, int starty, int startx, const char* title)
{
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	rectangle(starty - 1, startx - 1, starty + height, startx + width + 1);
	mvprintw(starty - 1, startx + (width - static_cast<int>(strlen(title))) / 2, title);
	attroff(COLOR_PAIR(1));
}