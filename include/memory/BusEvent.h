/*
 * BusMsg.h
 *
 *  Created on: 25 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef BUSMSG_H_
#define BUSMSG_H_

struct BusEvent {
	int event;
	int address;
	int value;
	int sender;
};

#endif /* BUSMSG_H_ */
