/*
 * Core.cpp
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <processor/Core.h>
#include <iostream>
#include <chrono>

int Core::nextID = 0;

/**
 * Constructor that sets the Core ID
 * @param pCoreID Core ID
 */
Core::Core(int pChipID, int pCoreID, Instruction_gen *pGenerator, Logger *pLogger) {
	_chipID = pChipID;
	_coreID = pCoreID;
	_generator = pGenerator;
	_logger = pLogger;
	_cacheL1 = new CacheL1(_chipID, _coreID, _logger);
	_running = false;
	_globalID = nextID++;
}

/**
 * Destructor
 */
Core::~Core() {
	delete _cacheL1;
}

/**
 * Sets the ID of the Core
 *
 * @param pCoreID Core ID (0/1/...)
 */
void Core::setCoreID(int pCoreID) {
	_coreID = pCoreID;
}

/**
 * Returns the Core ID
 *
 * @return Core ID
 */
int Core::getCoreID() {
	return _coreID;
}

void Core::work() {
	while (_running) {
		Instruction_gen::Instruction inst;
		_generator->getNew(inst);
		std::string msg = "Chip: " + std::to_string(_chipID) + " Core: " + std::to_string(_coreID) + " Inst: "
				+ std::to_string(inst.op) + " Data: " + std::to_string(inst.data) + "\n";

		_logger->write(msg);
		manage(inst);
	}

}

/**
 * Starts the thread
 * @return
 */
void Core::startCore() {
	_running = true;
	_thread = std::thread(&Core::work, this);
}

/**
 * Stops the thread
 */
void Core::stopCore() {
	_running = false;
	_thread.join();
	_logger->write("Chip: " + std::to_string(_chipID) + " Core: " + std::to_string(_coreID) + " STOPPED... \n");
	_cacheL1->print();
}

void Core::manage(Instruction_gen::Instruction &pInstruction) {
	switch (pInstruction.op) {
	case cons::inst::TYPES::READ:
		std::this_thread::sleep_for(std::chrono::seconds((int) cons::BASE_TIME * cons::multipliers::READ));
		break;

	case cons::inst::TYPES::CALC:
		std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::CALC));
		break;

	case cons::inst::TYPES::WRITE:
		_cacheL1->write(pInstruction.dest, pInstruction.data);
		std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));
		break;
	}
}
