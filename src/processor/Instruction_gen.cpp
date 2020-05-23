/*
 * Instruction_gen.cpp
 *
 *  Created on: 14 may. 2020
 *      Author: juanp1995
 */

#include <processor/Instruction_gen.h>

Instruction_gen::Instruction_gen() {
	_randGenerator = std::mt19937(time(NULL));
	_poissonDist = std::poisson_distribution<int>(POISSON_MEAN);
	_running = false;
}

Instruction_gen::~Instruction_gen() {
}

/**
 * Thread function that keeps generating instructions for
 * all the cores.
 *
 * Instructions are stored in a queue
 */
void Instruction_gen::generateInstructions() {
	while (_running) {
		int type = getInstructionType();
		int data = 0;
		if (type == cons::inst::TYPES::WRITE) {
			data = getRandomData();
		}

		Instruction inst;
		inst.op = type;
		inst.dest = getRandomAddress();
		inst.data = data;
		push(inst);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/**
 * Stores a new instruction in the queue
 *
 * @param pInstruction New instruction
 */
void Instruction_gen::push(Instruction pInstruction) {
	if ((int) _queue.size() > cons::inst::MAX_INST) {
		//std::cout << "QUEUE FULL... WAITING..." << std::endl;
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(pInstruction);
	//std::cout << "New instruction generated!!!" << std::endl;
	_cond.notify_one();
}

/**
 * Function called by all the core thread to get a new
 * instruction
 *
 * @param instruction Instruction that the Core will get
 */
void Instruction_gen::getNew(Instruction &instruction) {
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this] {return !_queue.empty();});
	instruction = _queue.front();
	_queue.pop();
}

/**
 * Starts the thread that generates instructions
 */
void Instruction_gen::start() {
	_running = true;
	_thread = std::thread(&Instruction_gen::generateInstructions, this);
}

/**
 * Stops the thread
 */
void Instruction_gen::stop() {
	_running = false;
	_thread.join();
}

/**
 * Generates a random number with a Poisson distribution
 * to determine the type of instruction.
 *
 * @return Integer that represents the type of instruction
 */
int Instruction_gen::getInstructionType() {
	int number = _poissonDist(_randGenerator);
	if (/*number >= 0 &&*/number < 7) //READ
		return cons::inst::TYPES::READ;
	else if (number >= 7 && number < 13) //CALC
		return cons::inst::TYPES::CALC;
	else if (number >= 13 /*&& number < 24*/) //WRITE
		return cons::inst::TYPES::WRITE;

	return 0;
}

/**
 * Generates a random memory address
 *
 * @return Memory address (0 to 15)
 */
int Instruction_gen::getRandomAddress() {
	static thread_local std::mt19937 *rng = nullptr;
	if (!rng) rng = new std::mt19937(time(NULL));
	std::uniform_int_distribution<int> distr(0, cons::memory::RAM_SIZE - 1);
	return distr(*rng);
}

/**
 * Generates a random integer that represents the data to be stored
 *
 * @return Random integer (0 to 65535)
 */
int Instruction_gen::getRandomData() {
	static thread_local std::mt19937 *rng = nullptr;
	if (!rng) rng = new std::mt19937(time(NULL));
	std::uniform_int_distribution<int> dist(0, cons::inst::MAX_DATA);
	return dist(*rng);
}

