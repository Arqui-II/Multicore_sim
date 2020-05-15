/*
 * Instruction_gen.h
 *
 *  Created on: 14 may. 2020
 *      Author: juanp1995
 */

#ifndef _INSTRUCTION_GEN_H
#define _INSTRUCTION_GEN_H

class Instruction_gen {

private:
	struct Instruction{
		int chip;
		int core;
		int op;
		int dest;
		int data;
	};


public:
	Instruction_gen();
	virtual ~Instruction_gen();

	Instruction getInst();
};

#endif /* _INSTRUCTION_GEN_H_ */
