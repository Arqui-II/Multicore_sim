/*
 * Message.h
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

class Message {
public:
	Message(const int pEvent, const int pAddress);
	virtual ~Message();

	int getEvent();
	int getAddress();

private:
	int _event;
	int _address;
};

#endif /* MESSAGE_H_ */
