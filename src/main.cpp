//============================================================================
// Name        : Multicore_sim.cpp
// Author      : Juan Pablo Brenes C.
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <bitset>
#include <instruction_gen/Instruction_gen.h>
#include <processor/Processor.h>
#include <logger/Logger.h>
#include <unistd.h>
#include <random>

int main() {

	Logger logger;
	Instruction_gen instGenerator;
	Processor chip0(&instGenerator, &logger);

	chip0.startCores();

	std::this_thread::sleep_for(std::chrono::seconds(30));

	chip0.stopCores();

	return 0;
}
