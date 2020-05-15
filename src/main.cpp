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

int main() {

	Instruction_gen *generator = new Instruction_gen();

	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!

	std::cout << generator->getInst().data << std::endl;

	delete generator;

	return 0;
}
