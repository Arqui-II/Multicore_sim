/*
 * Instruction_gen.h
 *
 *  Created on: 14 may. 2020
 *      Author: juanp1995
 */

#ifndef _INSTRUCTION_GEN_H
#define _INSTRUCTION_GEN_H

#include <random>
#include <time.h>
#include <functional>

class Instruction_gen {

private:
	const double POISSON_MEAN = 12;

	//std::random_device rd; // uniformly-distributed integer random number generator
	std::mt19937 _randGenerator; // mt19937: Pseudo-random number generation
	std::poisson_distribution<int> _poissonDist;

	int getInstructionType();
	int getRandomAddress();
	int getRandomData();

public:
	struct Instruction {
		int chip;
		int core;
		int op;
		int dest;
		int data;
	};

	Instruction_gen();
	virtual ~Instruction_gen();

	Instruction getInst(int pChipID, int pCoreID);
};

#endif /* _INSTRUCTION_GEN_H_ */
