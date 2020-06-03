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
#include <processor/Instruction_gen.h>
#include <memory/CacheL1.h>
#include <memory/CacheL2.h>
#include <memory/RAM.h>

class Display {

private:
	struct ProcMem {
		CacheL1::Block *cachesL1[cons::memory::L1_NUMBER];
		CacheL2::Block *cacheL2;
	};

	WINDOW *_P0_C0L1, *_P0_C1L1, *_P1_C0L1, *_P1_C1L1;
	WINDOW *_P0L2, *_P1L2;
	WINDOW *_RAM;

	std::thread _thread;

	int L1_WIN_SIZEY = 8;
	int L1_WIN_SIZEX = 30;
	int L2_WIN_SIZEY = 8;
	int L2_WIN_SIZEX = 40;
	int RAM_WIN_SIZEY = 20;
	int RAM_WIN_SIZEX = 20;

	volatile bool _running;
	int _mainX, _mainY;
	int _L1X, _L1Y;

	ProcMem _processors[cons::NUMBER_OF_CHIPS];
	Instruction_gen::Instruction *_coreInstructions[cons::NUMBER_OF_CHIPS * cons::NUMBER_OF_CORES];
	volatile bool *_runningCores[cons::NUMBER_OF_CHIPS * cons::NUMBER_OF_CORES];
	RAM::Block *_ram;

	void printHeaders();
	std::string getInstuctionString(Instruction_gen::Instruction *pInst);
	void printL1();
	void printL2();
	void printRAM();
	void refreshWindows();

	void init();
	void exitRoutine();
	void update();

public:
	Display();
	virtual ~Display();

	void start();
	void stop();

	void setChipCache(CacheL1 **pL1Array, CacheL2 *pL2, int pChipID);
	void setSystemRAM(RAM::Block *pRam);
	void setCoreInfo(Instruction_gen::Instruction *pInst, volatile bool *pRunning, const int pChipID,
			const int pCoreID);

};

#endif /* DISPLAY_H_ */
