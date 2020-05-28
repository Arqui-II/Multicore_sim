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
	_runningL1 = false;

}

Controller::~Controller() {
}

void Controller::monitorL1() {
	while (_runningL1) {
		//BusEvent event = this->getEvent();
		//std::cout << "Controller received event from: " << std::to_string(event.sender) << std::endl;
	}
}

void Controller::start() {
	_runningL1 = true;
	_threadL1 = std::thread(&Controller::monitorL1, this);
}

void Controller::stop() {
	_runningL1 = false;
	_threadL1.join();
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

//BusEvent Controller::getEvent() {
//	BusEvent event = _l1BusEvents.front();
//	_l1BusEvents.pop();
//	return event;
//}

int Controller::requestL2(BusEvent pEvent) {
	std::lock_guard<std::mutex> lock(_mutexRead);
	int value = -1;
	if (_localL2->checkHit(pEvent.address)) { //DATA IS LOCATED IN THE LOCAL L2
		value = _localL2->localRequest(pEvent);
	}

	else if (_externalL2->checkHit(pEvent.address) && pEvent.event == cons::bus::EVENTS::READ_MISS) { //DATA IS LOCATED IN EXTERNAL L2
		value = _externalL2->externalRead(pEvent);
		this->_localL2->loadFromExternal(pEvent.address, value, pEvent.sender); //Write on local L2
	}

	else {
		value = _localL2->loadFromRAM(pEvent.address, pEvent.sender);
	}

	return value;
}

void Controller::writeRequestL2(int pAddress, int pData, int pOwner) {
	this->_localL2->writeThroughL1(pAddress, pData, pOwner);
}

int Controller::notify(BusEvent pEvent) {
	//_l1BusEvents.push(pEvent);
	//return _localL2->request(pEvent);
	return 0;
}

void Controller::notifyL1(BusEvent pEvent) {
	if (pEvent.sender == 0) {
		std::lock_guard<std::mutex> lock(_mutexNotifyL1);
		_memoriesL1[1]->notifyEvent(pEvent);
	}

	else {
		std::lock_guard<std::mutex> lock(_mutexNotifyL1);
		_memoriesL1[0]->notifyEvent(pEvent);
	}

}

int Controller::read(int pAddress) {

	return 0;
}

void Controller::notifyExternalL2(const BusEvent pEvent) {
	this->_externalL2->externalNotify(pEvent);
}

