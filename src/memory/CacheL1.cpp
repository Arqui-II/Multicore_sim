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
	for (auto itr : this->_cacheL1) {
		itr.data = 0;
		itr.ramAddress = 0;
		itr.state = cons::memory::L1_STATES::I;
		itr.valid = false;
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
		if (hit
				&& (block->state == cons::memory::L1_STATES::S || block->state == cons::memory::L1_STATES::M)) {
			value = read(pAddress % cons::memory::L1_SIZE);

		}

		else { //CACHE MISS OR BLOCK INVALID
			_controller->notifyL1(BusEvent { cons::bus::EVENTS::READ_MISS, pAddress, this->_coreID });
			value = _controller->requestL2(BusEvent { cons::bus::EVENTS::READ_MISS, pAddress, _coreID });
			this->write(pAddress, value, cons::memory::L1_STATES::S); //Write readed value in corresponding block
		}
		break;

	case cons::inst::WRITE:
		_controller->notifyL1(BusEvent { cons::bus::EVENTS::WRITE_MISS, pAddress, this->_coreID });
		_controller->writeRequestL2(pAddress, pData, this->_coreID); //Write on L2 too
		this->write(pAddress, pData, cons::memory::L1_STATES::M);
		//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));

//		if (hit
//				&& (block->state == cons::memory::L1_STATES::S || block->state == cons::memory::L1_STATES::M)) {
//			//NOTIFY THE WRITE TO THE BUS!!!!
//			_controller->notifyL1(BusEvent { cons::bus::EVENTS::WRITE_MISS, pAddress, this->_coreID });
//			_controller->writeRequestL2(pAddress, pData, this->_coreID); //Write on L2 too
//			this->write(pAddress, pData, cons::memory::L1_STATES::M);
//			//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));
//		} else {
//			//WRITE MISS
//			//NOTIFY THE WRITE MISS TO THE BUS
//			this->write(pAddress, pData, cons::memory::L1_STATES::M);
//			value = -2;
//			std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));
//		}

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
	_mutex.lock();
	int block = pAddress % cons::memory::L1_SIZE;

	_cacheL1[block].data = pData;
	_cacheL1[block].ramAddress = pAddress;
	_cacheL1[block].state = pState;
	_mutex.unlock();

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
//_busEvents.push(pEvent);
	for (auto blockItr : _cacheL1) {
		if (blockItr.ramAddress == pEvent.address) {
			switch (pEvent.event) {
			case cons::bus::EVENTS::READ_MISS:
				if (blockItr.state == cons::memory::L1_STATES::M) blockItr.state = cons::memory::L1_STATES::S;
				break;

			case cons::bus::EVENTS::WRITE_MISS:
				blockItr.state = cons::memory::L1_STATES::I;
				break;
			}
		}
	}
}

void CacheL1::setCurrentData(int pData) {
	_currentData = pData;
}

CacheL1::Block* CacheL1::getCacheL1(){
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
