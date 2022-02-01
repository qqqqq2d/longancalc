#include <curses.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include "ncfunctions.h"
#include "constants.h"
#include "calc_colors.h"
#include "print_calc.h"
#include "button.h"
#include "interface.h"
#include "computer.h"
#include <cmath>
#include "calculator.h"
#include "computer.h"



int main()
{
	computer c;
	interface<computer> ui(c);
	calculator calc(ui);

	while (true)
	{
		if(!calc.loop_step())
			break;
	}
	ui.end();
	return 0;
}




/*
	print_calc p(calc_win);
	p.add("katse1");
	p.add("katse2");
	p.add("katse3");
	p.add("katse4");
	int ch = getch();
	p.add("katse5");
	ch = getch();
	p.add("katse6");
	ch = getch();
	p.add("katse7");
	ch = getch();
*/
