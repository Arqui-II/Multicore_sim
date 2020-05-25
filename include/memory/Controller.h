/*
 * ControllerL2.h
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <thread>
#include <queue>
#include <Constants.h>
#include <memory/BusEvent.h>
#include <memory/CacheL1.h>
#include <memory/CacheL2.h>

class CacheL1;

class Controller {

protected:
	static int nextID; //Instance counter

public:
	Controller();
	virtual ~Controller();

	int notify(BusEvent pEvent);
	void addL1(CacheL1 *pCacheL1, int pCoreID);
	void setLocalL2(CacheL2 *pLocalL2);
	void setExternalL2(CacheL2 *pExternalL2);

	void start();
	void stop();

private:
	int _id;
	bool _runningL1;
	std::thread _threadL1;

	CacheL1 *_memoriesL1[cons::memory::L1_NUMBER];
	CacheL2 *_localL2;
	CacheL2 *_externalL2;
	//std::queue<BusEvent> _l1BusEvents;

	int read(int pAddress);

	void notifyL1(BusEvent pEvent, int pDest);
	//BusEvent getEvent();
	void monitorL1();

};

#endif /* CONTROLLER_H_ */
