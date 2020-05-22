/*
 * Core.h
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CORE_H_
#define CORE_H_

#include <thread>
#include <instruction_gen/Instruction_gen.h>
#include <logger/Logger.h>

class Core {

private:
	int _coreID, _chipID;
	std::thread _thread;
	void work();

	Instruction_gen *_generator;
	Logger *_logger;

	static const int TIME = 4;
	bool _running;

public:
	Core(int pChipID, int pCoreID, Instruction_gen *pGenerator, Logger *pLogger);
	virtual ~Core();

	void setCoreID(int pCoreID);
	int getCoreID();

	int startCore();
	int stopCore();
};

#endif /* CORE_H_ */
