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
	noecho();
	getmaxyx(stdscr, _mainY, _mainX);
	printw("Multicore system simulation.   2 chips, 4 cores");

	_P0_C0L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, 0);
	_P0_C1L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEX + 1));
	_P1_C0L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEX * 2) + 1);
	_P1_C1L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEX * 3) + 1);

	box(_P0_C0L1, 0, 0);
	wrefresh(_P0_C0L1);

	box(_P0_C1L1, 0, 0);
	wrefresh(_P0_C1L1);

	box(_P1_C0L1, 0, 0);
	wrefresh(_P1_C0L1);

	box(_P1_C1L1, 0, 0);
	wrefresh(_P1_C1L1);
	refresh();

//	mvwprintw(_P0_C0L1, 1, 1, "Test!!!");
//	wrefresh(_P0_C0L1);
//
//	mvwprintw(_P1_C0L1, 1, 1, "Test2!!!");
//	wrefresh(_P1_C0L1);
}

void Display::start() {
	_running = true;
	_thread = std::thread(&Display::update, this);
}

void Display::stop() {
	_running = false;
	_thread.join();
}

void Display::setChipL1(CacheL1 **pL1Array, int pChipID) {
	if (pChipID == 0) {
		_chip0L1[0] = pL1Array[0]->getCacheL1();
		_chip0L1[1] = pL1Array[1]->getCacheL1();
	} else {
		_chip1L1[0] = pL1Array[0]->getCacheL1();
		_chip1L1[1] = pL1Array[1]->getCacheL1();
	}
}

void Display::printL1() {
	//PRINTS L1 CHIP 0 CORE 0
	mvwprintw(_P0_C0L1, 1, 1, "Chip 0 -- Core 0");
	mvwprintw(_P0_C0L1, 2, 1, "Block|State|Address|Data");
	for (int i = 3; i < 5; i++) {
		std::stringstream tmp;
		tmp << std::setw(5) << std::setfill(' ') << std::left << i - 3 << "|" << std::setw(5)
				<< cons::logger::STATES_L1[_chip0L1[0][i - 3].state] << "|" << std::setw(7) << std::hex
				<< _chip0L1[0][i - 3].ramAddress << std::dec << "|" << std::setw(4) << std::hex
				<< _chip0L1[0][i - 3].data;
		mvwprintw(_P0_C0L1, i, 1, tmp.str().c_str());
	}
	//PRINTS L1 CHIP 0 CORE 1
	mvwprintw(_P0_C1L1, 1, 1, "Chip 0 -- Core 1");
	mvwprintw(_P0_C1L1, 2, 1, "Block|State|Address|Data");
	for (int i = 3; i < 5; i++) {
		std::stringstream tmp;
		tmp << std::setw(5) << std::setfill(' ') << std::left << i - 3 << "|" << std::setw(5)
				<< cons::logger::STATES_L1[_chip0L1[1][i - 3].state] << "|" << std::setw(7) << std::hex
				<< _chip0L1[1][i - 3].ramAddress << std::dec << "|" << std::setw(4) << std::hex
				<< _chip0L1[1][i - 3].data;
		mvwprintw(_P0_C1L1, i, 1, tmp.str().c_str());
	}
	//PRINTS L1 CHIP 1 CORE 0
	mvwprintw(_P1_C0L1, 1, 1, "Chip 1 -- Core 0");
	mvwprintw(_P1_C0L1, 2, 1, "Block|State|Address|Data");
	for (int i = 3; i < 5; i++) {
		std::stringstream tmp;
		tmp << std::setw(5) << std::setfill(' ') << std::left << i - 3 << "|" << std::setw(5)
				<< cons::logger::STATES_L1[_chip1L1[0][i - 3].state] << "|" << std::setw(7) << std::hex
				<< _chip1L1[0][i - 3].ramAddress << std::dec << "|" << std::setw(4) << std::hex
				<< _chip1L1[0][i - 3].data;
		mvwprintw(_P1_C0L1, i, 1, tmp.str().c_str());
	}
	//PRINTS L1 CHIP 1 CORE 1
	mvwprintw(_P1_C1L1, 1, 1, "Chip 1 -- Core 1");
	mvwprintw(_P1_C1L1, 2, 1, "Block|State|Address|Data");
	for (int i = 3; i < 5; i++) {
		std::stringstream tmp;
		tmp << std::setw(5) << std::setfill(' ') << std::left << i - 3 << "|" << std::setw(5)
				<< cons::logger::STATES_L1[_chip1L1[1][i - 3].state] << "|" << std::setw(7) << std::hex
				<< _chip1L1[1][i - 3].ramAddress << std::dec << "|" << std::setw(4) << std::hex
				<< _chip1L1[1][i - 3].data;
		mvwprintw(_P1_C1L1, i, 1, tmp.str().c_str());
	}
}

void Display::update() {
	while (_running) {
		this->printL1();

		wrefresh(_P0_C0L1);
		wrefresh(_P0_C1L1);
		wrefresh(_P1_C0L1);
		wrefresh(_P1_C1L1);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
