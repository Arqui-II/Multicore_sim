/*
 * Core.h
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CORE_H_
#define CORE_H_

#include <thread>

class Core {

private:
	int _coreID;
	std::thread _thread;
	void work();

public:
	Core();
	Core(int pCoreID);
	virtual ~Core();

	void setCoreID(int pCoreID);
	int getCoreID();

	int startCore();
	int stopCore();
};

#endif /* CORE_H_ */
