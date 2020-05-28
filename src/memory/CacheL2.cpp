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
	// TODO Auto-generated destructor stub
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
		block->owners[1].chip = -1;

	}
}

bool CacheL2::checkHit(int pAddress) {
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
int CacheL2::localRequest(BusEvent pEvent) {
	int value = -1; //Undefined
	Block *block = &_cacheL2[pEvent.address % cons::memory::L2_SIZE];

	switch (pEvent.event) {
	case cons::bus::EVENTS::READ_MISS:
		if (block->state == cons::memory::L2_STATES::DS) {
			value = block->data;
			this->updateOwners(block, pEvent.sender);
		} else {
			//_controller->notifyL1(pEvent);
			value = block->data;
			block->state = cons::memory::L2_STATES::DS;
			this->updateOwners(block, pEvent.sender);
		}
		break;

	case cons::bus::EVENTS::WRITE_MISS:
		if (block->state == cons::memory::L2_STATES::DS) {
			//_controller->notifyL1(pEvent);
			this->uniqueOwner(block, pEvent.sender);
			block->external = false;
			block->data = pEvent.value;
			block->state = cons::memory::L2_STATES::DM;
			_ram->write(pEvent.address, pEvent.value, this->_ID); //On miss, write on RAM
		} else {
			//_controller->notifyL1(pEvent);
			this->uniqueOwner(block, pEvent.sender);
			block->external = false;
			block->data = pEvent.value;
			_ram->write(pEvent.address, pEvent.value, this->_ID); //On miss, write on RAM
		}
		break;
	}

	if (block->external) _controller->notifyExternalL2(pEvent);

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
	Block *block = &_cacheL2[pEvent.address % cons::memory::L2_SIZE];
	if (pEvent.event == cons::bus::EVENTS::WRITE_MISS) {
		block->state = cons::memory::L2_STATES::DI;
		block->external = false;
		this->deleteOwners(block);
	}
}

void CacheL2::loadFromExternal(int pAddress, int pData, int pOwner) {
	Block *block = &this->_cacheL2[pAddress % cons::memory::L2_SIZE];

	//If a block has to be replaced, first we need to write the current data to RAM
	if (block->ramAddress != pAddress && block->state != cons::memory::L2_STATES::DI) {
		this->_ram->write(block->ramAddress, block->data, this->_ID);
	}
	//If the block was shared with the other L2, notify it about the WRITE
	if (block->external) {
		_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_MISS, block->ramAddress });
	}

	block->data = pData;
	block->ramAddress = pAddress;
	block->state = cons::memory::L2_STATES::DM;
	block->external = true;
	this->uniqueOwner(block, pOwner);
}

int CacheL2::loadFromRAM(int pAddress, int pOwner) {
	int value = -1;
	value = this->_ram->read(pAddress, pOwner);
	Block *block = &this->_cacheL2[pAddress % cons::memory::L2_SIZE];

	//If a block has to be replaced, first we need to write the current data to RAM
	if (block->ramAddress != pAddress && block->state != cons::memory::L2_STATES::DI) {
		this->_ram->write(block->ramAddress, block->data, this->_ID);
	}
	//If the block was shared with the other L2, notify it about the WRITE
	if (block->external) {
		_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_MISS, block->ramAddress });
	}

	block->ramAddress = pAddress;
	block->data = value;
	block->external = false;
	block->state = cons::memory::L2_STATES::DM;
	this->uniqueOwner(block, pOwner);

	return value;
}

void CacheL2::writeThroughL1(int pAddress, int pData, int pOwner) {
	Block *block = &this->_cacheL2[pAddress % cons::memory::L2_SIZE];

	//If a block has to be replaced, first we need to write the current data to RAM
	if (block->ramAddress != pAddress && block->state != cons::memory::L2_STATES::DI) {
		this->_ram->write(block->ramAddress, block->data, this->_ID);
	}
	//If the block was shared with the other L2, notify it about the WRITE
	if (block->external) {
		_controller->notifyExternalL2(BusEvent { cons::bus::EVENTS::WRITE_MISS, block->ramAddress });
	}

	block->ramAddress = pAddress;
	block->data = pData;
	block->external = false;
	block->state = cons::memory::L2_STATES::DM;
	this->uniqueOwner(block, pOwner);
}

void CacheL2::deleteOwners(Block *pBlock) {
	for (auto itr : pBlock->owners) {
		itr.chip = -1;
		itr.core = -1;
	}

}

void CacheL2::updateOwners(Block *pBlock, int pOwner) {
	pBlock->owners[pOwner].chip = this->_ID; //CacheL2 ID = Chip ID
	pBlock->owners[pOwner].core = pOwner;
}

void CacheL2::uniqueOwner(Block *pBlock, int pOwner) {
	pBlock->owners[pOwner].chip = this->_ID;
	pBlock->owners[pOwner].core = pOwner;

	pBlock->owners[pOwner ^ 1].chip = -1;
	pBlock->owners[pOwner ^ 1].core = -1;
}

