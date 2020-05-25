/*
 * CacheL1.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/CacheL1.h>

CacheL1::CacheL1(Controller *pController, int pChipID, int pCoreID, Logger *pLogger) {
	_chipID = pChipID;
	_coreID = pCoreID;
	_logger = pLogger;
	_controller = pController;
	_controller->addL1(this, _coreID);
	_currentData = -1; //Undefined
	_running = false;
}

CacheL1::~CacheL1() {
}

void CacheL1::busMonitor() {
	while (_running) {
		if (!_busEvents.empty()) {
			BusEvent event = this->getEvent();
			std::cout << "Received event from: " << std::to_string(event.sender) << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void CacheL1::start() {
	_running = true;
	_thread = std::thread(&CacheL1::busMonitor, this);
}

void CacheL1::stop() {
	_running = false;
	_thread.join();
}

//void CacheL1::onNotify(Message pMessage) {
//	std::cout << "Message received in Core: " << std::to_string(_coreID) << " Address: "
//			<< std::to_string(pMessage.getAddress()) << std::endl;
//}

int CacheL1::request(int pOp, int pAddress, int pData) {
	bool hit = false;
	int value = -1; //Undefined
	Block *block = nullptr;
	for (auto blockItr : _cacheL1)
		if (blockItr.ramAddress == pAddress) {
			hit = true;
			block = &blockItr;
		}

	switch (pOp) {
	case cons::inst::READ:
		if (hit && block->valid) {
			//CACHE HIT!!!!
			value = read(pAddress % cons::memory::L1_SIZE);
		} else {
			//READ MISS
			value = _controller->notify(BusEvent { cons::bus::EVENTS::READ_MISS, block->ramAddress, _coreID });
		}
		break;

	case cons::inst::WRITE:
		if (hit) {
			//CACHE HIT!!!!
			std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));
		} else {
			//WRITE MISS
			std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));
		}

		break;
	}

	return value;
}

int CacheL1::read(int pBlock) {
	return _cacheL1[pBlock].data;
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

BusEvent CacheL1::getEvent() {
	BusEvent event = _busEvents.front();
	_busEvents.pop();
	return event;
}

void CacheL1::notifyEvent(BusEvent pEvent) {
	_busEvents.push(pEvent);
}

void CacheL1::setCurrentData(int pData) {
	_currentData = pData;
}

void CacheL1::print() {
	std::cout << "Contents of cacheL1 of Core: " << std::to_string(_coreID) << std::endl;
	for (int i = 0; i < cons::memory::L1_SIZE; ++i) {
		Block block = _cacheL1[i];
		std::cout << "Block: " << std::to_string(i) << " State: " << std::to_string(block.state) << " Address: "
				<< std::to_string(block.ramAddress) << " Data: " << std::to_string(block.data) << std::endl;
	}
}

