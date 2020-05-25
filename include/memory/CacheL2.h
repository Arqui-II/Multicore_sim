/*
 * CacheL2.h
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CACHEL2_H_
#define CACHEL2_H_

#include <vector>
#include <Constants.h>
#include <memory/BusEvent.h>

class CacheL2 {

protected:
	static int nextID; //Counter of instances

private:
	struct Owners {
		int chip;
		int core;
		bool externBlock;
	};

	struct Block {
		int state;
		std::vector<Owners> owners;
		int ramAddress;
		int data;
	};

	int _ID; //Matches with the processor ID

	Block _cacheL2[cons::memory::L2_SIZE];

public:
	CacheL2();
	virtual ~CacheL2();

	int request(BusEvent pEvent);
};

#endif /* CACHEL2_H_ */
