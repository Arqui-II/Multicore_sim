/*
 * CacheL1.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CACHEL1_H_
#define CACHEL1_H_

#include <mutex>
#include <Constants.h>
#include <logger/Logger.h>

class CacheL1 {

private:
	struct Block {
		int state;
		int ramAddress;
		int data;
	};

	int _chipID, _coreID;
	Block _cacheL1[cons::memory::L1_SIZE];
	Logger *_logger;

	std::mutex _mutex;

public:
	CacheL1(int pChipID, int pCoreID, Logger *pLogger);
	virtual ~CacheL1();

	int write(int pAddress, int pData);
	void print();
};

#endif /* CACHEL1_H_ */
