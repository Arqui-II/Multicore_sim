/*
 * Core.h
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CORE_H_
#define CORE_H_

#include <thread>
#include <sstream>
#include <string>
#include <Constants.h>
#include <processor/Instruction_gen.h>
#include <memory/CacheL1.h>
#include <memory/Controller.h>
#include <logger/Logger.h>

class Core {

protected:
	static int nextID;

private:
	int _coreID, _chipID;
	int _globalID;
	volatile bool _running;
	bool _working;
	std::thread _thread;

	Instruction_gen *_generator;
	Instruction_gen::Instruction _currentInst;
	Logger *_logger;

	CacheL1 *_cacheL1;
	Controller *_controller;

	void work();
	int manage(Instruction_gen::Instruction &pInstruction);

	void printInst(const Instruction_gen::Instruction pInst);

public:
	Core(Controller *pController, int pChipID, int pCoreID, Instruction_gen *pGenerator, Logger *pLogger);
	virtual ~Core();

	void setCoreID(int pCoreID);
	int getCoreID();
	Instruction_gen::Instruction* getCurrentInst();
	volatile bool* getRunning();

	void startCore();
	void stopCore();
};

#endif /* CORE_H_ */
