/*
 * Processor.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <processor/Processor.h>

int Processor::counterID = 0;

Processor::Processor(Instruction_gen *pGenerator, Logger *pLogger) {
	_chipID = counterID++;
	_instructionGen = pGenerator;
	_logger = pLogger;
	_core0 = new Core(_chipID, 0, _instructionGen, _logger);
	_core1 = new Core(_chipID, 1, _instructionGen, _logger);
}

Processor::~Processor() {
	delete (_core0);
	delete (_core1);
}

void Processor::startCores() {
	_core0->startCore();
	_core1->startCore();
}

void Processor::stopCores() {
	_core0->stopCore();
	_core1->stopCore();
}

