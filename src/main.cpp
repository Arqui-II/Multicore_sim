//============================================================================
// Name        : Multicore_sim.cpp
// Author      : Juan Pablo Brenes C.
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <processor/Instruction_gen.h>
#include <memory/RAM.h>
#include <processor/Processor.h>
#include <logger/Logger.h>
#include <display/Display.h>
#include <unistd.h>

int main() {

	//std::cout << "Multicore system simulation..." << std::endl << std::endl;

	Display display;
	Logger logger;
	Instruction_gen instGenerator;
	RAM ram;
	Processor chip0(&instGenerator, &ram, &logger);
	Processor chip1(&instGenerator, &ram, &logger);
	chip0.setControllerExtL2(chip1.getCacheL2());
	chip1.setControllerExtL2(chip0.getCacheL2());

	display.setChipL1(chip0._controller.getL1(), 0);
	display.setChipL1(chip1._controller.getL1(), 1);

	instGenerator.start();
	chip0.startCores();
	chip1.startCores();
	display.start();

	std::this_thread::sleep_for(std::chrono::seconds(5));

	chip0.stopCores();
	chip1.stopCores();
	display.stop();
	instGenerator.stop();

	return 0;
}
