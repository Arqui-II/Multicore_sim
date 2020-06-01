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

public:
	struct Block {
		int owners[cons::NUMBER_OF_CORES];
		int data;
	};

	RAM();
	virtual ~RAM();

	int read(int pAddress, int pOwner);
	void write(int pAddress, int pData, int pOwner);

	Block* getRam();

private:

	std::mutex _mutex;

	Block _ram[cons::memory::RAM_SIZE];

	void init();

};

#endif /* RAM_H_ */
