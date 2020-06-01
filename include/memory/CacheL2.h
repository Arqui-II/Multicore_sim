/*
 * CacheL2.h
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CACHEL2_H_
#define CACHEL2_H_

#include <vector>
#include <mutex>
#include <Constants.h>
#include <memory/BusEvent.h>
#include <memory/Controller.h>
#include <memory/RAM.h>

class Controller;

class CacheL2 {

protected:
	static int nextID; //Counter of instances

public:
	struct Owner {
		int chip;
		int core;
	};

	struct Block {
		int state;
		Owner owners[2];
		bool external;
		int ramAddress;
		int data;
	};
	CacheL2(RAM *pRam, Controller *pController);
	virtual ~CacheL2();

	bool checkHit(int pAddress);
	int localRead(BusEvent pEvent, bool pSetExternal = false);
	int externalRead(BusEvent pEvent);
	void externalNotify(BusEvent pEvent);
	void internalNotify(BusEvent pEvent);
	void loadFromExternal(int pAddress, int pData, int pOwner);
	int loadFromRAM(BusEvent pEvent);
	void localWrite(BusEvent pEvent, const bool pExternalHit);

	CacheL2::Block* getL2();

private:
	RAM *_ram;
	Controller *_controller;

	int _ID; //Matches with the processor ID
	std::mutex _mutex;

	Block _cacheL2[cons::memory::L2_SIZE];

	void init();
	void deleteOwners(Block *pBlock);
	void deleteOneOwner(Block *&pBlock, const int pOwner);
	void updateOwners(Block *&pBlock, int pOwner);
	void uniqueOwner(Block *&pBlock, int pOwner);

};

#endif /* CACHEL2_H_ */
