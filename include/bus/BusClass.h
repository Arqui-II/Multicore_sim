///*
// * BusClass.h
// *
// *  Created on: 25 may. 2020
// *      Author: Juan Pablo Brenes C.
// */
//
//#ifndef BUSCLASS_H_
//#define BUSCLASS_H_
//
//#include <functional>
//#include <bus/BusL1.h>
//
//class BusClass {
//public:
//	BusClass(BusL1 *pBusL1) {
//		this->_busL1 = pBusL1;
//		this->_busL1->addParticipant(this->getNotifyFunc());
//	}
//
//	virtual void update() {
//	}
//	;
//
//protected:
//	BusL1 *_busL1;
//	std::function<void(Message)> getNotifyFunc() {
//		auto listener = [=](Message message) -> void {
//			this->onNotify(message);
//		};
//		return listener;
//	}
//
//	void send(Message pMessage) {
//		_busL1->sendMessage(pMessage);
//	}
//
//	virtual void onNotify(Message pMessage) {
//	}
//	;
//};
//
//#endif /* BUSCLASS_H_ */
