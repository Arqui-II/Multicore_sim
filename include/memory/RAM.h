/*
 * RAM.h
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef RAM_H_
#define RAM_H_

#include <thread>
#include <mutex>
#include <Constants.h>

class RAM {

private:
	struct Block {
		int owners[cons::NUMBER_OF_CORES];
		int data;
	};

	std::mutex _mutex;

	Block _ram[cons::memory::RAM_SIZE];

	void init();

public:
	RAM();
	virtual ~RAM();

	int read(int pAddress, int pOwner);
	void write(int pAddress, int pData, int pOwner);

};

#endif /* RAM_H_ */
