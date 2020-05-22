/*
 * Instruction_gen.cpp
 *
 *  Created on: 14 may. 2020
 *      Author: juanp1995
 */

#include <instruction_gen/Instruction_gen.h>

Instruction_gen::Instruction_gen() {
	_randGenerator = std::mt19937(time(NULL));
	_poissonDist = std::poisson_distribution<int>(POISSON_MEAN);
}

Instruction_gen::~Instruction_gen() {
}

/**
 * Generates a random number with a Poisson distribution
 * to determine the type of instruction.
 *
 * @return Integer that represents the type of instruction
 */
int Instruction_gen::getInstructionType() {
	int number = _poissonDist(_randGenerator);
	if (number >= 0 && number < 8) //READ
		return 0;
	else if (number >= 8 && number < 16) //CALC
		return 1;
	else if (number >= 16 && number < 24) //WRITE
		return 2;
	else
		return getInstructionType();
}

int Instruction_gen::getRandomAddress() {
	std::uniform_int_distribution<int> distr(0, 16);
	return distr(_randGenerator);
}

int Instruction_gen::getRandomData() {
	std::uniform_int_distribution<int> distr(0, 65535);
	return distr(_randGenerator);
}

Instruction_gen::Instruction Instruction_gen::getInst(int pChipID, int pCoreID) {
	int type = getInstructionType();
	int data = 0;
	if (type == 2) data = getRandomData();

	Instruction inst;
	inst.chip = pChipID;
	inst.core = pCoreID;
	inst.op = getInstructionType();
	inst.dest = getRandomAddress();
	inst.data = data;

	return inst;

}

