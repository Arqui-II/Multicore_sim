///*
// * BusL1.cpp
// *
// *  Created on: 24 may. 2020
// *      Author: Juan Pablo Brenes C.
// */
//
//#include <bus/BusL1.h>
//
//BusL1::BusL1() {
//	_running = false;
//}
//
//BusL1::~BusL1() {
//}
//
//void BusL1::addParticipant(std::function<void(Message)> pParticipant) {
//	_participants.push_back(pParticipant);
//}
//
//void BusL1::sendMessage(Message pMessage) {
//	_bus.push(pMessage);
//}
//
//void BusL1::notify() {
//	while (_running) {
//		if (!_bus.empty()) {
//			for (auto itr = _participants.begin(); itr != _participants.end(); itr++) {
//				(*itr)(_bus.front());
//			}
//			_bus.pop();
//		}
//	}
//}
//
//void BusL1::start() {
//	_running = true;
//	_thread = std::thread(&BusL1::notify, this);
//}
//
//void BusL1::stop() {
//	_running = false;
//	_thread.join();
//}
