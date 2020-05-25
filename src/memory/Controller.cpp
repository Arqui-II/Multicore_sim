/*
 * ControllerL2.cpp
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/Controller.h>

int Controller::nextID = 0; //Instance counter

Controller::Controller() {
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

void Controller::addL1(CacheL1 *pCacheL1, int pCoreID) {
	_memoriesL1[pCoreID] = pCacheL1;
}

void Controller::setLocalL2(CacheL2 *pLocalL2) {
	this->_localL2 = pLocalL2;
}

void Controller::setExternalL2(CacheL2 *pExternalL2) {
	this->_externalL2 = pExternalL2;
}

//BusEvent Controller::getEvent() {
//	BusEvent event = _l1BusEvents.front();
//	_l1BusEvents.pop();
//	return event;
//}

int Controller::notify(BusEvent pEvent) {
	//_l1BusEvents.push(pEvent);
	return _localL2->request(pEvent);
}

void Controller::notifyL1(BusEvent pEvent, int pDest) {
	_memoriesL1[pDest]->notifyEvent(pEvent);
}

int Controller::read(int pAddress) {

}

