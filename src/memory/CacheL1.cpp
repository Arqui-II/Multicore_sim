/*
 * CacheL1.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/CacheL1.h>

CacheL1::CacheL1(Controller *pController, int pChipID, int pCoreID, Logger *pLogger) :
		_controller(pController), _chipID(pChipID), _coreID(pCoreID), _logger(pLogger) {
	_controller->addL1(this, _coreID);
	_currentData = -1; //Undefined
	_running = false;
	this->init();
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

void CacheL1::init() {
	for (int i = 0; i < cons::memory::L1_SIZE; i++) {
		Block *block = &_cacheL1[i];
		block->data = 0;
		block->ramAddress = 0;
		block->state = cons::memory::L1_STATES::I;
		block->valid = false;
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
	for (int i = 0; i < cons::memory::L1_SIZE; i++) {
		if (_cacheL1[i].ramAddress == pAddress) {
			hit = true;
			block = &_cacheL1[i];
		}
	}

	switch (pOp) {
	case cons::inst::READ:
		if (hit
				&& (block->state == cons::memory::L1_STATES::S || block->state == cons::memory::L1_STATES::M)) {
			value = read(pAddress % cons::memory::L1_SIZE);
		}

		else { //CACHE MISS OR BLOCK INVALID
			   //_controller->notifyL1(BusEvent { cons::bus::EVENTS::READ_MISS, pAddress, 0, _coreID });
			   //std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::L2));
			value = _controller->readRequestL2(
					BusEvent { cons::bus::EVENTS::READ_MISS, pAddress, 0, _coreID });

			//Block *oldBlock = &_cacheL1[pAddress % cons::memory::L1_SIZE];
			//Notify L2 about replacement of block
//			_controller->notifyL2(
//					BusEvent { cons::bus::EVENTS::WRITE_BACK, oldBlock->ramAddress, 0, _coreID });
			this->write(pAddress, value, cons::memory::L1_STATES::S); //Write readed value in corresponding block
		}
		break;

	case cons::inst::WRITE:
		if (hit
				&& (block->state == cons::memory::L1_STATES::S || block->state == cons::memory::L1_STATES::M)) {
			//_controller->notifyL1(BusEvent { cons::bus::EVENTS::WRITE_HIT, pAddress, pData, this->_coreID });
			_controller->writeRequestL2(
					BusEvent { cons::bus::EVENTS::WRITE_HIT, pAddress, pData, this->_coreID }); //Write through L2
			this->write(pAddress, pData, cons::memory::L1_STATES::M);
		}

		else {
			//_controller->notifyL1(BusEvent { cons::bus::EVENTS::WRITE_MISS, pAddress, pData, this->_coreID });
			//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::L2));
			_controller->writeRequestL2(
					BusEvent { cons::bus::EVENTS::WRITE_MISS, pAddress, pData, this->_coreID }); //Write through L2
			this->write(pAddress, pData, cons::memory::L1_STATES::M);
		}
		break;
	}

	return value;
}

/**
 * Reads a block of the cache L1 and return the value
 *
 * @param pBlock Block where the data is stored
 * @return Data int
 */
int CacheL1::read(int pBlock) {
	return _cacheL1[pBlock].data;
}

int CacheL1::write(int pAddress, int pData, int pState) {
	//std::lock_guard<std::mutex> lock(_mutex);
	int block = pAddress % cons::memory::L1_SIZE;

	_cacheL1[block].data = pData;
	_cacheL1[block].ramAddress = pAddress;
	_cacheL1[block].state = pState;

	return 0;
}

BusEvent CacheL1::getEvent() {
	BusEvent event = _busEvents.front();
	_busEvents.pop();
	return event;
}

/**
 * Handles the events originated in the other core
 *
 * @param pEvent Event originated in the other core (READ/WRITE MISS)
 */
void CacheL1::notifyEvent(BusEvent pEvent) {
	Block *block = &_cacheL1[pEvent.address % cons::memory::L1_SIZE];
	if (block->ramAddress == pEvent.address) {
		switch (pEvent.event) {
		case cons::bus::EVENTS::READ_MISS:
			if (block->state == cons::memory::L1_STATES::M) block->state = cons::memory::L1_STATES::S;
			break;

		case cons::bus::EVENTS::WRITE_MISS:
			block->state = cons::memory::L1_STATES::I;
			break;

		case cons::bus::EVENTS::WRITE_HIT:
			block->state = cons::memory::L1_STATES::I;
			break;
		}
	}
}

void CacheL1::setCurrentData(int pData) {
	_currentData = pData;
}

CacheL1::Block* CacheL1::getCacheL1() {
	return _cacheL1;
}

void CacheL1::print() {
	std::cout << "Contents of cacheL1 of Core: " << std::to_string(_coreID) << std::endl;
	for (int i = 0; i < cons::memory::L1_SIZE; ++i) {
		Block block = _cacheL1[i];
		std::cout << "Block: " << std::to_string(i) << " State: " << std::to_string(block.state)
				<< " Address: " << std::to_string(block.ramAddress) << " Data: " << std::to_string(block.data)
				<< std::endl;
	}
}
