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
		//Instruction_gen::Instruction inst;
		_generator->getNew(this->_chipID, this->_coreID, _currentInst);
//		std::string msg = "Chip: " + std::to_string(inst.chip) + " Core: " + std::to_string(inst.core)
//				+ " Inst: " + cons::logger::TYPES[inst.op] + " Data: " + std::to_string(inst.data) + "\n";

//_logger->write(msg);
		//this->printInst(_currentInst);
		int value = manage(_currentInst);
//		if (inst.op == cons::inst::TYPES::READ)
//			std::cout << "P" << _chipID << "," << _coreID << " " << "Readed value: " << std::to_string(value)
//					<< std::endl;
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
	_logger->write(
			"Chip: " + std::to_string(_chipID) + " Core: " + std::to_string(_coreID) + " STOPPED... \n");
}

int Core::manage(Instruction_gen::Instruction &pInstruction) {
	int value = -1;
	switch (pInstruction.op) {
	case cons::inst::TYPES::READ:
		value = _cacheL1->request(pInstruction.op, pInstruction.dest, pInstruction.data);
		std::this_thread::sleep_for(std::chrono::seconds((int) cons::BASE_TIME * cons::multipliers::READ));
		break;

	case cons::inst::TYPES::CALC:
		std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::CALC));
		break;

	case cons::inst::TYPES::WRITE:
		value = _cacheL1->request(pInstruction.op, pInstruction.dest, pInstruction.data);
		std::this_thread::sleep_for(std::chrono::seconds(cons::BASE_TIME * cons::multipliers::WRITE));
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
