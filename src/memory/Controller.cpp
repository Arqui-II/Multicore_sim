/*
 * ControllerL2.cpp
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/Controller.h>

int Controller::nextID = 0; //Instance counter

Controller::Controller() :
		_localL2(nullptr), _externalL2(nullptr) {
	_id = nextID++;

}

Controller::~Controller() {
}

/**
 * Adds the L1 caches in the current chip
 *
 * @param pCacheL1 CacheL1 class pointer
 * @param pCoreID ID of the core who owns the L1
 */
void Controller::addL1(CacheL1 *pCacheL1, int pCoreID) {
	_memoriesL1[pCoreID] = pCacheL1;
}

/**
 * Sets the cache L2 of the current chip
 *
 * @param pLocalL2 CacheL2 class pointer
 */
void Controller::setLocalL2(CacheL2 *pLocalL2) {
	this->_localL2 = pLocalL2;
}

/**
 * Sets the cache L2 of the other chip
 *
 * @param pExternalL2 CacheL2 class pointer
 */
void Controller::setExternalL2(CacheL2 *pExternalL2) {
	this->_externalL2 = pExternalL2;
}

CacheL1** Controller::getL1() {
	return _memoriesL1;
}

CacheL2* Controller::getL2() {
	return _localL2;
}

int Controller::readRequestL2(BusEvent pEvent) {
	std::lock_guard<std::mutex> lock(_mutex);
	int value = -1;
	bool externalHit = _externalL2->checkHit(pEvent.address);

	this->notifyL1(pEvent);
	if (_localL2->checkHit(pEvent.address)) { //DATA IS LOCATED IN THE LOCAL L2
		if (externalHit)
			_externalL2->externalNotify(BusEvent { cons::bus::EVENTS::READ_HIT, pEvent.address });
		value = _localL2->localRead(pEvent, externalHit);
	}

	else if (externalHit && pEvent.event == cons::bus::EVENTS::READ_MISS) { //DATA IS LOCATED IN EXTERNAL L2
		value = _externalL2->externalRead(pEvent);
		//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::L2));
		this->_localL2->loadFromExternal(pEvent.address, value, pEvent.sender); //Write on local L2
	}

	else { //LOAD DATA FROM RAM
		std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::RAM));
		value = _localL2->loadFromRAM(pEvent);
	}

	return value;
}

void Controller::writeRequestL2(BusEvent pEvent) {
	std::lock_guard<std::mutex> lock(_mutex);
	bool externalHit = _externalL2->checkHit(pEvent.address);
	this->notifyL1(pEvent);
	this->_localL2->localWrite(pEvent, externalHit);
}

void Controller::notifyL1(BusEvent pEvent) {
	//std::lock_guard<std::mutex> lock(_mutex);
	if (pEvent.sender == 0) {
		//std::lock_guard<std::mutex> lock(_mutex);
		_memoriesL1[1]->notifyEvent(pEvent);
	}

	else if (pEvent.sender == 1) {
		//std::lock_guard<std::mutex> lock(_mutex);
		_memoriesL1[0]->notifyEvent(pEvent);
	}

	else { //Notify all caches
		   //std::lock_guard<std::mutex> lock(_mutex);
		_memoriesL1[0]->notifyEvent(pEvent);
		_memoriesL1[1]->notifyEvent(pEvent);
	}

}

void Controller::notifyExternalL2(const BusEvent pEvent) {
	this->_externalL2->externalNotify(pEvent);
}

void Controller::notifyL2(const BusEvent pEvent) {
	std::lock_guard<std::mutex> lock(_mutex);
	_localL2->internalNotify(pEvent);
}
