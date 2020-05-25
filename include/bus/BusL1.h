///*
// * BusL1.h
// *
// *  Created on: 24 may. 2020
// *      Author: Juan Pablo Brenes C.
// */
//
//#ifndef BUSL1_H_
//#define BUSL1_H_
//
//#include <functional>
//#include <queue>
//#include <vector>
//#include <thread>
//#include <chrono>
//#include <bus/Message.h>
//
//class BusL1 {
//public:
//	BusL1();
//	virtual ~BusL1();
//
//	void addParticipant(std::function<void(Message)> pParticipant);
//	void sendMessage(Message pMessage);
//	void notify();
//
//	void start();
//	void stop();
//
//private:
//	std::vector<std::function<void(Message)>> _participants;
//	std::queue<Message> _bus;
//
//	bool _running;
//	std::thread _thread;
//};
//
//#endif /* BUSL1_H_ */
