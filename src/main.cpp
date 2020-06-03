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

	display.setCoreInfo(chip0.getCore(0)->getCurrentInst(), chip0.getCore(0)->getRunning(), 0, 0);
	display.setCoreInfo(chip0.getCore(1)->getCurrentInst(), chip0.getCore(1)->getRunning(), 0, 1);
	display.setCoreInfo(chip1.getCore(0)->getCurrentInst(), chip1.getCore(0)->getRunning(), 1, 0);
	display.setCoreInfo(chip1.getCore(1)->getCurrentInst(), chip1.getCore(1)->getRunning(), 1, 1);
	display.setChipCache(chip0.getController()->getL1(), chip0.getController()->getL2(), 0);
	display.setChipCache(chip1.getController()->getL1(), chip1.getController()->getL2(), 1);
	display.setSystemRAM(ram.getRam());

	instGenerator.start();
	display.start();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	chip0.startCores();
	chip1.startCores();

	std::this_thread::sleep_for(std::chrono::seconds(30));

	chip0.stopCores();
	chip1.stopCores();

	std::this_thread::sleep_for(std::chrono::seconds(5));
	display.stop();
	instGenerator.stop();

	return 0;
}
