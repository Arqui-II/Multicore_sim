/*
 * Message.cpp
 *
 *  Created on: 24 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <bus/Message.h>

Message::Message(const int pEvent, const int pAddress) {
	_event = pEvent;
	_address = pAddress;
}

Message::~Message() {
}

int Message::getEvent() {
	return _event;
}

int Message::getAddress() {
	return _address;
}

