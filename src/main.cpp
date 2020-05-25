//============================================================================
// Name        : Multicore_sim.cpp
// Author      : Juan Pablo Brenes C.
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <bitset>
#include <processor/Instruction_gen.h>
#include <processor/Processor.h>
#include <logger/Logger.h>
#include <unistd.h>
#include <random>
#include <limits.h>

int main() {

	Logger logger;
	Instruction_gen instGenerator;
	Processor chip0(&instGenerator, &logger);
//	Processor chip1(&instGenerator, &logger);
//	chip0.setControllerExtL2(chip1.getCacheL2());
//	chip1.setControllerExtL2(chip0.getCacheL2());

	instGenerator.start();
	chip0.startCores();
	//chip1.startCores();

	std::this_thread::sleep_for(std::chrono::seconds(15));

	chip0.stopCores();
	instGenerator.stop();
	//chip1.stopCores();

	return 0;
}
