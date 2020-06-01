/*
 * Core.cpp
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <iostream>
#include <chrono>
#include <processor/Core.h>

int Core::nextID = 0;

/**
 * Constructor that sets the Core ID
 * @param pCoreID Core ID
 */
Core::Core(Controller *pController, int pChipID, int pCoreID, Instruction_gen *pGenerator, Logger *pLogger) {
	_chipID = pChipID;
	_coreID = pCoreID;
	_generator = pGenerator;
	_logger = pLogger;
	_controller = pController;
	_cacheL1 = new CacheL1(pController, _chipID, _coreID, _logger);
	_running = false;
	_working = false;
	_globalID = nextID++;
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
	while (_running) {
		_generator->getNew(this->_chipID, this->_coreID, _currentInst);
		manage(_currentInst);
	}
}

/**
 * Starts the thread
 * @return
 */
void Core::startCore() {
	_running = true;
	_thread = std::thread(&Core::work, this);
	_working = true;
}

/**
 * Stops the thread
 */
void Core::stopCore() {
	_running = false;
	_thread.join();
	_working = false;
	_logger->write(
			"Chip: " + std::to_string(_chipID) + " Core: " + std::to_string(_coreID) + " STOPPED... \n");
}

Instruction_gen::Instruction* Core::getCurrentInst() {
	return &_currentInst;
}

volatile bool* Core::getRunning() {
	return &_working;
}

int Core::manage(Instruction_gen::Instruction &pInstruction) {
	int value = -1;
	switch (pInstruction.op) {
	case cons::inst::TYPES::READ:
		//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::L1));
		value = _cacheL1->request(pInstruction.op, pInstruction.dest, pInstruction.data);
		break;

	case cons::inst::TYPES::CALC:
		std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::CALC));
		break;

	case cons::inst::TYPES::WRITE:
		//std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * (int) cons::multipliers::L1));
		value = _cacheL1->request(pInstruction.op, pInstruction.dest, pInstruction.data);
		break;
	}

	return value;
}

void Core::printInst(const Instruction_gen::Instruction pInst) {
	std::stringstream out;
	out << "P" << pInst.chip << "," << pInst.core << " ";
	out << cons::logger::TYPES[pInst.op];
	switch (pInst.op) {
	case cons::inst::TYPES::CALC:
		out << std::endl;
		break;
	case cons::inst::TYPES::READ:
		out << " " << pInst.dest << std::endl;
		break;
	case cons::inst::TYPES::WRITE:
		out << " " << pInst.dest << ", " << pInst.data << std::endl;
		break;
	}

	std::cout << out.str();

}
