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
class CacheL2;

class Controller {

protected:
	static int nextID; //Instance counter

public:
	Controller();
	virtual ~Controller();

	void addL1(CacheL1 *pCacheL1, int pCoreID);
	void setLocalL2(CacheL2 *pLocalL2);
	void setExternalL2(CacheL2 *pExternalL2);

	void notifyL1(BusEvent pEvent);
	void notifyExternalL2(const BusEvent pEvent);
	void notifyL2(const BusEvent pEvent);
	int readRequestL2(BusEvent pEvent);
	void writeRequestL2(BusEvent pEvent);

	CacheL1** getL1();
	CacheL2* getL2();

private:
	int _id;
	std::mutex _mutex;

	CacheL1 *_memoriesL1[cons::memory::L1_NUMBER];
	CacheL2 *_localL2;
	CacheL2 *_externalL2;

};

#endif /* CONTROLLER_H_ */
