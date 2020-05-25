/*
 * CacheL2.cpp
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <memory/CacheL2.h>

int CacheL2::nextID = 0;

CacheL2::CacheL2() {
	_ID = nextID++;

}

CacheL2::~CacheL2() {
	// TODO Auto-generated destructor stub
}

int CacheL2::request(BusEvent pEvent) {

	//IMPLEMENT DIRECTORIES PROTOCOL!!!!
	return 0;
}

