/*
 * Processor.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <processor/Processor.h>

int Processor::counterID = 0;

Processor::Processor(Instruction_gen *pGenerator, Logger *pLogger) : _controller(), _cacheL2() {
	_chipID = counterID++;
	_instructionGen = pGenerator;
	_logger = pLogger;
	_controller.setLocalL2(&_cacheL2);
	_core0 = new Core(&_controller, _chipID, 0, _instructionGen, _logger);
	_core1 = new Core(&_controller, _chipID, 1, _instructionGen, _logger);
}

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

void Processor::startCores() {
//_busL1.start();
	_core0->startCore();
	_core1->startCore();
}

void Processor::stopCores() {
	_core0->stopCore();
	_core1->stopCore();
//_busL1.stop();
}

