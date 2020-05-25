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
#include <thread>
#include <Constants.h>
#include <logger/Logger.h>
#include <bus/BusClass.h>
#include <memory/BusEvent.h>
#include <memory/Controller.h>
//#include <bus/BusL1.h>

class Controller;

class CacheL1 {

private:
	struct Block {
		int state;
		bool valid;
		int ramAddress;
		int data;
	};

	int _chipID, _coreID;
	int _currentData;
	bool _running;
	Block _cacheL1[cons::memory::L1_SIZE];
	std::queue<BusEvent> _busEvents;
	Logger *_logger;
	Controller *_controller;

	std::thread _thread;
	std::mutex _mutex;

	//void onNotify(Message pMessage);
	void busMonitor();
	BusEvent getEvent();
	int write(int pAddress, int pData);
	int read(int pAddress);

public:
	CacheL1(Controller *pController, int pChipID, int pCoreID, Logger *pLogger);
	virtual ~CacheL1();

	int request(int pOp, int pAddress, int pData = 0);
	void notifyEvent(BusEvent pEvent);

	void setCurrentData(int pData);

	void start();
	void stop();

	void print();
};

#endif /* CACHEL1_H_ */
