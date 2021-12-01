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
        initscr();
        int height = 6;
        int width = 20;
        int starty = 5;
        int startx = 0;
        printw("calc");
        local_win = newwin(height, width, starty, startx);
        scrollok(local_win, TRUE);
        char fun;
        for (int i = 0; i < 1000; i++)
        {
                auto key = getch();
                if (key == 'q') break;



                bool found = false;
                for (size_t i = 0; i < 18; i++)
                {
                if(allowed[i]==key)
                        found=true;
                }
    
                if (!found)
                        continue;
    
                if(key == '+' || key == '*')
                {
                fun = key;
                strcpy(a_buf, buf);
                //printf("a:%s\n", a_buf);
                }

                if(key == '=')
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
                
                wprintw(local_win, "=%.10g\n", result);
                r_index = 0;
                //continue;
                }

                if(r_index>=18)
                        continue;

                if((key == 'C') && (r_index>0))
                {
                char cbuf[20];
                sprintf(cbuf, "                   ");
                r_index--;      
                buf[r_index] = '\0';
                }
                else if((key == 'L') && (r_index>0))
                {
                sprintf(buf, "                   ");
                r_index=0;
                }
                else 
                {
                buf[r_index] = key;
                ++r_index;    
                buf[r_index] = '\0';
                }

                char buf_print[20];
                sprintf(buf_print, "%c", key);
                wprintw(local_win, "%s", buf_print);
                //wprintw(local_win, "%d123456789123456789\n", i);

                wrefresh(local_win);
        }
        getch();
        endwin();

        return 0;
}
