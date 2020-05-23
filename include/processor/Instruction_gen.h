/*
 * Instruction_gen.h
 *
 *  Created on: 14 may. 2020
 *      Author: juanp1995
 */

#ifndef _INSTRUCTION_GEN_H
#define _INSTRUCTION_GEN_H

#include <iostream>
#include <random>
#include <time.h>
#include <functional>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <Constants.h>

class Instruction_gen {

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

	void start();
	void stop();
	void getNew(Instruction &instruction);

private:
	const double POISSON_MEAN = 10.1;

	std::mt19937 _randGenerator; // mt19937: Pseudo-random number generation
	std::poisson_distribution<int> _poissonDist;

	std::thread _thread;
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<Instruction> _queue;
	bool _running;

	int getInstructionType();
	int getRandomAddress();
	int getRandomData();

	void push(Instruction pInstruction);
	void generateInstructions();
};

#endif /* _INSTRUCTION_GEN_H_ */
