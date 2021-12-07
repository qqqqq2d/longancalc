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

int main(int argc, char *argv[])
{
        WINDOW *local_win;
        WINDOW *debug_win;
        initscr();
        int height = 6;
        int width = 20;
        int starty = 5;
        int startx = 0;        
        local_win = newwin(height, width, starty, startx);

        scrollok(local_win, TRUE);
        char fun;
        noecho();
        for (int i = 0; i < 1000; i++)
        {
                auto key = getch();
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

                //char buf_print[20];
                //sprintf(buf_print, "%c", key);
                //wprintw(local_win, "%s", buf_print);

                //wprintw(local_win, "%d123456789123456789\n", i);

                wrefresh(local_win);
        }
        //getch();
        endwin();

        return 0;
}
