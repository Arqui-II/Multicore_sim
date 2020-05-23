/*
 * Core.h
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CORE_H_
#define CORE_H_

#include <thread>
#include <Constants.h>
#include <processor/Instruction_gen.h>
#include <memory/CacheL1.h>
#include <logger/Logger.h>

class Core {

protected:
	static int nextID;

private:
	int _coreID, _chipID;
	int _globalID;
	bool _running;
	std::thread _thread;

	Instruction_gen *_generator;
	CacheL1 *_cacheL1;
	Logger *_logger;

	void work();
	void manage(Instruction_gen::Instruction &pInstruction);

public:
	Core(int pChipID, int pCoreID, Instruction_gen *pGenerator, Logger *pLogger);
	virtual ~Core();

	void setCoreID(int pCoreID);
	int getCoreID();

	void startCore();
	void stopCore();
};

#endif /* CORE_H_ */
