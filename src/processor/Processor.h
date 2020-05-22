/*
 * Processor.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <core/Core.h>
#include <instruction_gen/Instruction_gen.h>
#include <logger/Logger.h>

class Processor {

protected:
	static int counterID;

private:
	int _chipID;

	Instruction_gen *_instructionGen;
	Logger *_logger;
	Core *_core0, *_core1;

public:
	Processor(Instruction_gen *pGenerator, Logger *pLogger);
	virtual ~Processor();

	void startCores();
	void stopCores();
};

#endif /* PROCESSOR_H_ */
