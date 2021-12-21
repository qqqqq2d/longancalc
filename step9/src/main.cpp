#include <curses.h>
#include <string.h>
#include <stdlib.h>
constexpr auto allowed =".0123456789+-*/=CLq";

char buf[20];
char ibuf[20];

char a_buf[20];
char b_buf[20];

int r_index;

double a;
double b;
double result;

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

int main(int argc, char *argv[])
{
	initscr();

	keypad(stdscr, TRUE);
	// Don't mask any mouse events
	//mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

        int height = 6;
        int width = 20;
        int starty = 15;
        int startx = 1;
        WINDOW* input_win = newwin(1, 1, 0, 0);
        WINDOW* local_win = newwin(height, width, starty, startx);
        rectangle_around_window(height, width, starty, startx);

        constexpr int debug_win_height = 8;
        constexpr int debug_win_width = 40;
        WINDOW* debug_win = newwin(debug_win_height, debug_win_width, 2, startx);
        rectangle_around_window(debug_win_height, debug_win_width, 2, startx, "Debug");

        constexpr int help_win_height = 20;
        constexpr int help_win_width = 40;
        WINDOW* help_win = newwin(help_win_height, help_win_width, 2, 60);
        rectangle_around_window(help_win_height, help_win_width, 2, 60, "Help");
        scrollok(help_win, TRUE);

        refresh();
        scrollok(debug_win, TRUE);
        //mvwprintw(debug_win, 1, 0, "debug");
		mvwprintw(debug_win, 3, 0, "Ver:%s", __VERSION__);
        wrefresh(debug_win);
        //auto k = getch();
        //refresh();

        scrollok(local_win, TRUE);
        char fun;
        noecho();
	char buffer[512];
	size_t max_size = sizeof(buffer);
        for (int i = 0; i < 1000; i++)
        {
			const auto key = wgetch(input_win);
//			const auto key = getch();
			if(key == ERR) continue;
				if (key == KEY_MOUSE)
				{
					MEVENT event;
					if (getmouse(&event) == OK)
					{
						mvwprintw(debug_win, 4, 0, "x:%d, y:%d", event.x, event.y);
						wrefresh(debug_win);
					}
					continue;
				}
                mvwprintw(debug_win, 0, 0, "key:%c", key);
                wrefresh(debug_win);
                if (key == 'q') break;


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
                mvwprintw(debug_win, 1, 0, "r_index:%d", r_index);
                wrefresh(debug_win);

                //char buf_print[20];
                //sprintf(buf_print, "%c", key);
                //wprintw(local_win, "%s", buf_print);

                //wprintw(local_win, "%d123456789123456789\n", i);

                wrefresh(local_win);

        }
        //getch();
		printf("\033[?1003l\n"); // Disable mouse movement events, as l = low
        endwin();

        return 0;
}
