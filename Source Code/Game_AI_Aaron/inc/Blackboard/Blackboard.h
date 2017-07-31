#pragma once
#include <glm\vec2.hpp>
#include <vector>

class GameObject;

/** A singleton class to broadcast messages
* @author Jack McCall
* NB: With assistance from Ash*/
class Blackboard {
public:
	enum class MessageType { None, EnemySpotted, Position };

	struct Message {
	public:
		Message() {

		}

		MessageType type;
		long ID;
		GameObject *author, *target;
		float lifespan;
		bool remove;

	};

	/** Create a new singleton instance - must be matched with a call to 'destroy()'*/
	static Blackboard* create();
	/** Free up the memory for the singleton instance*/
	static void destroy();
	/** Retrieve an instance of the singleton*/
	static Blackboard* getInstance();

	/** Broadcast a message to the blackboard*/
	void postMessage(Message &m);

	/** Get the next available message ID*/
	long getNextMessageID();

	/** Get a list of all broadcast messages*/
	//const std::vector<Message> &getMessages();

	template <typename MsgType>
	std::vector<Message> getMessagesOfType();

private:
	Blackboard();
	~Blackboard();

	static Blackboard* sm_instance;
	std::vector<Message> m_messages;
	long m_nextID;
};

template<typename MsgType>
inline std::vector<Blackboard::Message> Blackboard::getMessagesOfType() {
	std::vector<Message> tmpMsgList;
	for (auto iter = m_messages.begin(), iter != m_messages.end(), iter++) {
		if (dynamic_cast<MsgType>((*iter).type) != nullptr) {
			tmpMsgList.push_back((*iter));
		}
	}
	return tmpMsgList;
}
