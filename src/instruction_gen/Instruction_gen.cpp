/*
 * Instruction_gen.cpp
 *
 *  Created on: 14 may. 2020
 *      Author: juanp1995
 */

#include "instruction_gen/Instruction_gen.h"

Instruction_gen::Instruction_gen() {
}

Instruction_gen::~Instruction_gen() {
}

Instruction_gen::Instruction Instruction_gen::getInst(){
	Instruction inst;
	inst.chip = 0;
	inst.core = 0;
	inst.op = 2;
	inst.dest = 15;
	inst.data = 4;

	return inst;

}

