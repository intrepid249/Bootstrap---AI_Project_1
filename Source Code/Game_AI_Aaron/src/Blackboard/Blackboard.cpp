#include "Blackboard\Blackboard.h"

Blackboard* Blackboard::sm_instance = nullptr;

Blackboard * Blackboard::create() {
	if (sm_instance == nullptr) sm_instance = new Blackboard();
	return sm_instance;
}

void Blackboard::destroy() {
	delete sm_instance;
	sm_instance = nullptr;
}

Blackboard * Blackboard::getInstance() {
	return sm_instance;
}

void Blackboard::postMessage(Message & m) {
}

long Blackboard::getNextMessageID() {
	return 0;
}

//const std::vector<Blackboard::Message>& Blackboard::getMessages() {
//	// TODO: insert return statement here
//}

Blackboard::Blackboard() {
}

Blackboard::~Blackboard() {
}
