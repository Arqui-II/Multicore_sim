/*
 * CacheL1.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/CacheL1.h>

CacheL1::CacheL1(int pChipID, int pCoreID, Logger *pLogger) {
	_chipID = pChipID;
	_coreID = pCoreID;
	_logger = pLogger;

}

CacheL1::~CacheL1() {
	// TODO Auto-generated destructor stub
}

int CacheL1::write(int pAddress, int pData) {
	_mutex.lock();
	int block = pAddress % cons::memory::L1_SIZE;

	_cacheL1[block].data = pData;
	_cacheL1[block].ramAddress = pAddress;
	_cacheL1[block].state = cons::memory::L1_STATES::S;
	_mutex.unlock();

	return 0;
}

void CacheL1::print() {
	std::cout << "Contents of cacheL1 of Core: " << std::to_string(_coreID) << std::endl;
	for (int i = 0; i < cons::memory::L1_SIZE; ++i) {
		Block block = _cacheL1[i];
		std::cout << "Block: " << std::to_string(i) << " State: " << std::to_string(block.state) << " Address: "
				<< std::to_string(block.ramAddress) << " Data: " << std::to_string(block.data) << std::endl;
	}
}

