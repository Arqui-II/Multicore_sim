/*
 * CacheL1.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CACHEL1_H_
#define CACHEL1_H_

#include <mutex>
#include <queue>
#include <Constants.h>
#include <logger/Logger.h>
#include <memory/BusEvent.h>
#include <memory/Controller.h>

class Controller;

class CacheL1 {

public:
	struct Block {
		int state;
		int ramAddress;
		int data;
	};

	CacheL1(Controller *pController, int pChipID, int pCoreID, Logger *pLogger);
	virtual ~CacheL1();

	int request(int pOp, int pAddress, int pData = 0);
	void notifyEvent(BusEvent pEvent);

	Block* getCacheL1();

	void start();
	void stop();
	void print();

private:
	Controller *_controller;
	int _chipID, _coreID;
	Logger *_logger;

	Block _cacheL1[cons::memory::L1_SIZE];
	std::mutex _mutex;

	void init();
	int write(int pAddress, int pData, int pState);
	int read(int pAddress);
};

#endif /* CACHEL1_H_ */
