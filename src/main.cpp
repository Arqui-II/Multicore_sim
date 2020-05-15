//============================================================================
// Name        : Multicore_sim.cpp
// Author      : Juan Pablo Brenes C.
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <bitset>
#include <core/Core.h>

int main() {

	Core core(0);

	core.startCore();

	for(int i=0; i<3; i++)
		std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!

	core.stopCore();


	return 0;
}
