#include "Behaviours\BehaviourController.h"
#include "Behaviours\Behaviour.h"

BehaviourController::BehaviourController() {
}

BehaviourController::~BehaviourController() {
}

void BehaviourController::update(float deltaTime) {
	processCommands();

	for (auto iter = m_behaviours.begin(); iter != m_behaviours.end(); iter++) {
		if ((*iter).second != nullptr)
			(*iter).second->doActions(deltaTime);
	}
}

void BehaviourController::render(aie::Renderer2D * renderer) {
	if (!m_behaviours.empty()) {
		for (auto iter = m_behaviours.begin(); iter != m_behaviours.end(); iter++) {
			if ((*iter).second != nullptr)
				(*iter).second->debugRender(renderer);
		}
	}
}

void BehaviourController::addBehaviour(std::shared_ptr<Behaviour> behaviour) {
	Command c;
	c.behaviour = behaviour;
	c.cmd = ADD;

	m_commands.push_back(c);
}

void BehaviourController::removeBehaviour(std::shared_ptr<Behaviour> behaviour) {
	Command c;
	c.behaviour = behaviour;
	c.cmd = REMOVE;

	m_commands.push_back(c);
}

const std::shared_ptr<Behaviour>& BehaviourController::getBehaviour(unsigned int ID) {
	auto iter = m_behaviours.find(ID);
	if (iter != m_behaviours.end())
		return m_behaviours.at(iter->first);

	return nullptr;
}

const std::unordered_map<unsigned int, std::shared_ptr<Behaviour>>& BehaviourController::getBehaviours() {
	return m_behaviours;
}

void BehaviourController::processCommands() {
	for (auto iter = m_commands.begin(); iter != m_commands.end(); iter++) {
		switch ((*iter).cmd) {
		case ADD:
			processAddBehaviour((*iter).behaviour);
			break;
		case REMOVE:
			processRemoveBehaviour((*iter).behaviour);
			break;
		}
	}
	m_commands.clear();
}

void BehaviourController::processAddBehaviour(std::shared_ptr<Behaviour> behaviour) {
	if (behaviour)
		behaviour->entryActions();

	m_behaviours[behaviour->getID()] = behaviour;
}

void BehaviourController::processRemoveBehaviour(std::shared_ptr<Behaviour> behaviour) {
	auto iter = m_behaviours.find(behaviour->getID());
	if (iter != m_behaviours.end())
		m_behaviours.erase(iter);
}
