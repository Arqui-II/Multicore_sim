/*
 * Core.cpp
 *
 *  Created on: 15 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include "Core.h"
#include <iostream>
#include <chrono>

/**
* Default constructor
*/
Core::Core(){
	_coreID = -1;
}

/**
* Constructor that sets the Core ID
* @param pCoreID Core ID
*/
Core::Core(int pCoreID){
	_coreID = pCoreID;
}

/**
* Destructor
*/
Core::~Core(){}

/**
* Sets the ID of the Core
*
* @param pCoreID Core ID (0/1/...)
*/
void Core::setCoreID(int pCoreID){
	_coreID = pCoreID;
}

/**
* Returns the Core ID
*
* @return Core ID
*/
int Core::getCoreID(){
	return _coreID;
}

void Core::work(){
	for(int i=0; i<5; i++){
		std::cout << "Working in Core: " << _coreID << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int Core::startCore(){
	_thread = std::thread(&Core::work, this);

	return 0;
}


int Core::stopCore(){
	_thread.join();
	return 0;
}

