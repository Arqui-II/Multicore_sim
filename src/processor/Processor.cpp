/*
 * Processor.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <processor/Processor.h>

int Processor::counterID = 0;

Processor::Processor(Instruction_gen *pGenerator, RAM *pRam, Logger *pLogger) :
		_instructionGen(pGenerator), _ram(pRam), _logger(pLogger), _cacheL2(_ram, &_controller) {
	_chipID = counterID++;
	_controller.setLocalL2(&_cacheL2);
	_core0 = new Core(&_controller, _chipID, 0, _instructionGen, _logger);
	_core1 = new Core(&_controller, _chipID, 1, _instructionGen, _logger);
}

/**
 * Destructor
 */
Processor::~Processor() {
	delete (_core0);
	delete (_core1);
}

void Processor::setControllerExtL2(CacheL2 *pExtL2) {
	_controller.setExternalL2(pExtL2);
}

CacheL2* Processor::getCacheL2() {
	return &this->_cacheL2;
}

Controller* Processor::getController() {
	return &_controller;
}

Core* Processor::getCore(int pCoreID) {
	if (pCoreID == 0)
		return _core0;
	else if (pCoreID == 1)
		return _core1;
	else
		return nullptr;
}

/**
 * Starts the thread of each cores
 */
void Processor::startCores() {
	_core0->startCore();
	_core1->startCore();
}

/**
 * Stops the thread of each cores
 */
void Processor::stopCores() {
	_core0->stopCore();
	_core1->stopCore();
}

