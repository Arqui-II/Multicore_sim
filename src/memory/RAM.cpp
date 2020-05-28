/*
 * RAM.cpp
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/RAM.h>

RAM::RAM() {
	this->init();
}

RAM::~RAM() {
}

/**
 * Initializes the RAM with 0s and undefined owners
 */
void RAM::init() {
	for (int i = 0; i < cons::memory::RAM_SIZE; i++) {
		_ram[i].data = 0;
		_ram[i].owners[0] = -1;
		_ram[i].owners[1] = -1;
	}
}

int RAM::read(int pAddress, int pOwner) {
	int value = this->_ram[pAddress].data;
	std::lock_guard<std::mutex> lock(_mutex);
	this->_ram[pAddress].owners[pOwner] = pOwner;
//	if (this->_ram[pAddress].owners[0] == -1 && this->_ram[pAddress].owners[1] == -1)
//		this->_ram[pAddress].owners[0] = pOwner;
//
//	else if (this->_ram[pAddress].owners[0] != -1 && this->_ram[pAddress].owners[0] != pOwner) {
//		if (this->_ram[pAddress].owners[1] != pOwner) this->_ram[pAddress].owners[1] = pOwner;
//	}
//
//	else if (this->_ram[pAddress].owners[0] == -1 && this->_ram[pAddress].owners[1] != pOwner)
//		this->_ram[pAddress].owners[0] = pOwner;

	return value;
}

void RAM::write(int pAddress, int pData, int pOwner) {
	std::lock_guard<std::mutex> lock(_mutex);
	this->_ram[pAddress].data = pData;
	this->_ram[pAddress].owners[0] = pOwner; //Chip who writes is the only owner
	this->_ram[pAddress].owners[1] = -1; //Delete other owner
}

