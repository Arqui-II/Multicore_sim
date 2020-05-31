/*
 * Display.cpp
 *
 *  Created on: 31 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <display/Display.h>

Display::Display() :
		_running(false) {

	this->init();
}

Display::~Display() {
	delwin(_P0_C0L1);
	delwin(_P0_C1L1);
	delwin(_P1_C0L1);
	delwin(_P1_C1L1);
	endwin();
}

void Display::init() {
	initscr();
	getmaxyx(stdscr, _mainY, _mainX);
	printw("Multicore system simulation.   2 chips, 4 cores");

	_P0_C0L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, 1);
	_P0_C1L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEY * 2) + 1);
	_P1_C0L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEY * 3) + 1);
	_P1_C1L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEY * 4) + 1);

	box(_P0_C0L1, 0, 0);
	wrefresh(_P0_C0L1);

	box(_P0_C1L1, 0, 0);
	wrefresh(_P0_C1L1);

	box(_P1_C0L1, 0, 0);
	wrefresh(_P1_C0L1);

	box(_P1_C1L1, 0, 0);
	wrefresh(_P1_C1L1);
	refresh();

	mvwprintw(_P0_C0L1, 1, 1, "Test!!!");
	wrefresh(_P0_C0L1);

	mvwprintw(_P1_C0L1, 1, 1, "Test2!!!");
	wrefresh(_P1_C0L1);
}

void Display::start() {
	_running = true;
	_thread = std::thread(&Display::update, this);
}

void Display::stop() {
	_running = false;
	_thread.join();
}

void Display::update() {
	while (_running) {

	}
}

