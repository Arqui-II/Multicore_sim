/*
 * Display.h
 *
 *  Created on: 31 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <curses.h>
#include <thread>
#include <chrono>

class Display {

private:
	WINDOW *_P0_C0L1, *_P0_C1L1, *_P1_C0L1, *_P1_C1L1;

	std::thread _thread;

	int L1_WIN_SIZEY = 4;
	int L1_WIN_SIZEX = 30;

	volatile bool _running;
	int _mainX, _mainY;
	int _L1X, _L1Y;

	void init();
	void update();

public:
	Display();
	virtual ~Display();

	void start();
	void stop();

};

#endif /* DISPLAY_H_ */
