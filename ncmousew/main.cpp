#include <curses.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include "ncfunctions.h"

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


char buf[20];
char ibuf[20];

char a_buf[20];
char b_buf[20];

int r_index;

double a;
double b;
double result;

int main()
{

	using namespace std::chrono_literals;
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
	constexpr int calc_win_height = 5;
	constexpr int calc_win_width = 20;
	WINDOW* local_win = newwin(calc_win_height, calc_win_width, 1, 5);
	rectangle_around_window(calc_win_height, calc_win_width, 1, 5, "CALC");
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

	char key;
	while(true)
	{
		MEVENT event;
		int ch = getch();
		key = 0;
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
					key = keys[y][x];
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
			key = ch;
		}
		if(key == 0) continue;
		mvwprintw(debug_win, 7, 0, "key:%c", key);
		wrefresh(debug_win);

		constexpr auto allowed =".0123456789+-*/=CLq";



                bool found = false;
                for (size_t i = 0; i < 18; i++)
                {
                        if(allowed[i]==key)
                                found=true;
                        if(key == 127 || key == 10)
                                found=true;
                }
    
                if (!found)
                        continue;
				char fun;
                if(key == '+' || key == '*' || key == '-' || key == '/')
                {
					fun = key;
					strcpy(a_buf, buf);
					//printf("a:%s\n", a_buf);
                }

                if(key == '=' || key == 10)
                {
					auto pos = strlen(a_buf);
					strcpy(b_buf, buf+pos+1);
							//printf("buf:%s len:%d b_buf:%s\n", buf, pos, b_buf);
					char *ptr;
					a = strtod(a_buf, &ptr);
					b = strtod(b_buf, &ptr);
					if (fun=='+')
					{
							result = a+b;
					}
					else if (fun=='*')
					{
							result = a*b;
					}
					else if (fun=='-')
					{
							result = a-b;
					}
					else if (fun=='/')
					{
							result = a/b;
					}
					auto count = width - r_index;
					for (size_t i = 0; i < count; i++)
					{
							mvwaddch(local_win,0,r_index+i, ' ');
					}
					
					mvwprintw(local_win, 1,0, "%.10g\n", result);
					r_index = 0;
					wrefresh(local_win);
					continue; 
                }

                if(r_index>=18)
                        continue;

                if((key == 'C' || key == 127) && (r_index>0))
                {
                        r_index--; 
                        mvwaddch(local_win,0,r_index, ' '); 
                        wmove(local_win, 0, r_index);                   
                        buf[r_index] = '\0';                        
                        wrefresh(local_win);
                        continue;                       
                }
                else if((key == 'L') && (r_index>0))
                {
                        r_index=0;
                        mvwprintw(local_win,0,0,"                   ");
                        wmove(local_win, 0, r_index);
                        wrefresh(local_win);
                        continue; 
                }
                else 
                {
                        buf[r_index] = key;
                        mvwaddch(local_win,0,r_index, key);
                        ++r_index;    
                        buf[r_index] = '\0';
                }

                //char buf_print[20];
                //sprintf(buf_print, "%c", key);
                //wprintw(local_win, "%s", buf_print);

                //wprintw(local_win, "%d123456789123456789\n", i);

                wrefresh(local_win);
	}

	printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
	endwin();
	return 0;
}

