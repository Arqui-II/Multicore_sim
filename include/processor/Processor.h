/*
 * Processor.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <processor/Core.h>
#include <processor/Instruction_gen.h>
#include <logger/Logger.h>
#include <memory/RAM.h>
#include <memory/Controller.h>
#include <memory/CacheL2.h>
//#include <bus/BusL1.h>

class Processor {

protected:
	static int counterID;

private:
	int _chipID;

	Instruction_gen *_instructionGen;
	RAM *_ram;
	Logger *_logger;
	Controller _controller;
	CacheL2 _cacheL2;
	Core *_core0, *_core1;

public:
	Processor(Instruction_gen *pGenerator, RAM *pRam, Logger *pLogger);
	virtual ~Processor();

	void setControllerExtL2(CacheL2 *pExtL2);
	CacheL2* getCacheL2();

	void startCores();
	void stopCores();
};

#endif /* PROCESSOR_H_ */
