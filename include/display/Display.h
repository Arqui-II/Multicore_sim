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
#include <iomanip>
#include <chrono>
#include <Constants.h>
#include <memory/CacheL1.h>
#include <memory/CacheL2.h>
#include <memory/RAM.h>

class Display {

private:
	WINDOW *_P0_C0L1, *_P0_C1L1, *_P1_C0L1, *_P1_C1L1;

	std::thread _thread;

	int L1_WIN_SIZEY = 5;
	int L1_WIN_SIZEX = 30;

	volatile bool _running;
	int _mainX, _mainY;
	int _L1X, _L1Y;

	CacheL1::Block *_chip0L1[cons::memory::L1_NUMBER];
	CacheL1::Block *_chip1L1[cons::memory::L1_NUMBER];

	void printL1();

	void init();
	void update();

public:
	Display();
	virtual ~Display();

	void start();
	void stop();

	void setChipL1(CacheL1 **pL1Array, int pChipID);

};

#endif /* DISPLAY_H_ */
