/*
 * CacheL2.cpp
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/CacheL2.h>

int CacheL2::nextID = 0;

CacheL2::CacheL2(RAM *pRam, Controller *pController) :
		_ram(pRam), _controller(pController) {
	_ID = nextID++;
	this->init();

}

CacheL2::~CacheL2() {
}

void CacheL2::init() {
	for (int i = 0; i < cons::memory::L2_SIZE; i++) {
		Block *block = &this->_cacheL2[i];
		block->data = 0;
		block->external = false;
		block->ramAddress = 0;
		block->state = cons::memory::L2_STATES::DI;
		block->owners[0].chip = -1;
		block->owners[0].core = -1;
		block->owners[1].chip = -1;
		block->owners[1].core = -1;

	}
}

bool CacheL2::checkHit(int pAddress) {
	std::lock_guard<std::mutex> lock(_mutex);
	bool hit = false;
	Block tmp = _cacheL2[pAddress % cons::memory::L2_SIZE];
	if (tmp.ramAddress == pAddress && tmp.state != cons::memory::L2_STATES::DI) hit = true;

	return hit;
}

/**
 * Called when its known that the data is stored in the cache
 * (from previous call to checkHit() )
 *
 * @param pEvent Event on the bus
 * @return
 */
int CacheL2::localRead(BusEvent pEvent, bool pSetExternal) {
	int value = -1; //Undefined
	Block *block = &_cacheL2[pEvent.address % cons::memory::L2_SIZE];

	if (block->state == cons::memory::L2_STATES::DS) {
		value = block->data;
		this->updateOwners(block, pEvent.sender);
	} else {
		value = block->data;
		block->state = cons::memory::L2_STATES::DS;
		this->updateOwners(block, pEvent.sender);
	}

	if (pSetExternal) block->external = true;

	return value;
}

/**
 * Called from the other chip, when it's known that there is a cache hit
 * (from previous call to checkHit() )
 *
 * @param pEvent Event on the bus
 * @return
 */
int CacheL2::externalRead(BusEvent pEvent) {
	int value = -1;

	Block *block = &_cacheL2[pEvent.address % cons::memory::L2_SIZE];

	if (pEvent.event == cons::bus::EVENTS::READ_MISS) {
		if (block->state == cons::memory::L2_STATES::DS) {
			value = block->data;
			block->external = true;
		} else {
			_controller->notifyL1(pEvent);
			value = block->data;
			block->state = cons::memory::L2_STATES::DS;
			block->external = true;
		}
	}

	return value;
}

void CacheL2::externalNotify(BusEvent pEvent) {
	std::lock_guard<std::mutex> lock(_mutex);
	Block *block = &_cacheL2[pEvent.address % cons::memory::L2_SIZE];
	if (block->ramAddress == pEvent.address) {
		if (pEvent.event == cons::bus::EVENTS::WRITE_HIT) {
			_controller->notifyL1(BusEvent { cons::bus::EVENTS::WRITE_HIT, pEvent.address, 0, -1 });
			block->state = cons::memory::L2_STATES::DI;
			block->external = false;
			this->deleteOwners(block);
		}
		//Block on external L2 was replaced
		else if (pEvent.event == cons::bus::EVENTS::WRITE_BACK) {
			block->external = false;
		}

		else if (pEvent.event == cons::bus::EVENTS::WRITE_MISS) {
			_controller->notifyL1(BusEvent { cons::bus::EVENTS::WRITE_MISS, pEvent.address, 0, -1 });
			block->state = cons::memory::L2_STATES::DI;
			block->external = false;
			this->deleteOwners(block);
		} else if (pEvent.event == cons::bus::EVENTS::READ_HIT) {
			block->external = true;
		}
	}
}

void CacheL2::internalNotify(BusEvent pEvent) {
	Block *block = &_cacheL2[pEvent.address % cons::memory::L2_SIZE];
	if (block->ramAddress == pEvent.address) {
		this->deleteOneOwner(block, pEvent.sender);
	}
}

void CacheL2::loadFromExternal(int pAddress, int pData, int pOwner) {
	Block *block = &this->_cacheL2[pAddress % cons::memory::L2_SIZE];

//If a block has to be replaced, first we need to write the current data to RAM
	if (block->ramAddress != pAddress) {
		if (block->state != cons::memory::L2_STATES::DI) {
			this->_ram->write(block->ramAddress, block->data, this->_ID);

			//If the block was shared with the other L2, notify it about the WRITE
			if (block->external) {
				_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_BACK, block->ramAddress });
			}
		}
	}

	else {
		if (block->external) {
			_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_HIT, block->ramAddress });
		}
	}

	block->data = pData;
	block->ramAddress = pAddress;
	block->state = cons::memory::L2_STATES::DS;
	block->external = true;
	this->uniqueOwner(block, pOwner);
}

int CacheL2::loadFromRAM(BusEvent pEvent) {
	//std::lock_guard<std::mutex> lock(_mutex);
	int value = -1;
	value = this->_ram->read(pEvent.address, pEvent.sender);
	Block *block = &this->_cacheL2[pEvent.address % cons::memory::L2_SIZE];

//If a block has to be replaced, first we need to write the current data to RAM
	if (block->state != cons::memory::L2_STATES::DI) {
		this->_ram->write(block->ramAddress, block->data, this->_ID);
	}
//If the block was shared with the other L2, notify it about the WRITE
	if (block->external) {
		_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_MISS, block->ramAddress });
	}

	block->ramAddress = pEvent.address;
	block->data = value;
	block->external = false;
	block->state = cons::memory::L2_STATES::DS;
	this->uniqueOwner(block, pEvent.sender);

	return value;
}

void CacheL2::localWrite(BusEvent pEvent, const bool pExternalHit) {
	Block *block = &this->_cacheL2[pEvent.address % cons::memory::L2_SIZE];

//If a block has to be replaced, first we need to write the current data to RAM
	if (block->ramAddress != pEvent.address) {
		if (block->state != cons::memory::L2_STATES::DI) {
			//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::RAM));
			this->_ram->write(block->ramAddress, block->data, this->_ID);

			//If the block was shared with the other L2, notify it about the WRITE
			if (block->external) {
				_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_BACK, block->ramAddress });
			}
		}
	}

	else {
		if (block->external) {
			_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_HIT, block->ramAddress });
		}
	}

	if (pExternalHit) _controller->notifyExternalL2(pEvent);

	block->ramAddress = pEvent.address;
	block->data = pEvent.value;
	block->external = false;
	block->state = cons::memory::L2_STATES::DM;
	this->uniqueOwner(block, pEvent.sender);
	std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::RAM));
	this->_ram->write(pEvent.address, pEvent.value, this->_ID); //WRITE THROUGH
}

CacheL2::Block* CacheL2::getL2() {
	return _cacheL2;
}

void CacheL2::deleteOwners(Block *pBlock) {
	for (int i = 0; i < 2; i++) {
		Owner *owner = &pBlock->owners[i];
		owner->chip = -1;
		owner->core = -1;
	}
}

void CacheL2::deleteOneOwner(Block *&pBlock, const int pOwner) {
	if (pBlock->owners[pOwner ^ 1].chip != -1) {
		pBlock->owners[pOwner].chip = -1; //CacheL2 ID = Chip ID
		pBlock->owners[pOwner].core = -1;
	}
}

void CacheL2::updateOwners(Block *&pBlock, int pOwner) {
	pBlock->owners[pOwner].chip = this->_ID; //CacheL2 ID = Chip ID
	pBlock->owners[pOwner].core = pOwner;
}

void CacheL2::uniqueOwner(Block *&pBlock, int pOwner) {
	pBlock->owners[pOwner].chip = this->_ID;
	pBlock->owners[pOwner].core = pOwner;

	pBlock->owners[pOwner ^ 1].chip = -1;
	pBlock->owners[pOwner ^ 1].core = -1;
}

