/*
 * Core.cpp
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <core/Core.h>
#include <iostream>
#include <chrono>

/**
 * Constructor that sets the Core ID
 * @param pCoreID Core ID
 */
Core::Core(int pChipID, int pCoreID, Instruction_gen *pGenerator, Logger *pLogger) {
	_chipID = pChipID;
	_coreID = pCoreID;
	_generator = pGenerator; //Function pointer
	_logger = pLogger;
	_running = false;
}

/**
 * Destructor
 */
Core::~Core() {
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
	std::this_thread::sleep_for(std::chrono::seconds(TIME));
	while (_running) {
		Instruction_gen::Instruction inst = _generator->getInst(_chipID, _coreID);
		std::string msg = "Chip: " + std::to_string(_chipID) + " Core: " + std::to_string(_coreID) + " Inst: "
				+ std::to_string(inst.op) + "\n";
		_logger->write(msg);
		//std::cout << "Inst " << std::to_string(inst.op) << " for core " << std::to_string(_coreID) << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

}

int Core::startCore() {
	_running = true;
	_thread = std::thread(&Core::work, this);

	return 0;
}

int Core::stopCore() {
	_running = false;
	_thread.join();
	return 0;
}

