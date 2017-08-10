#pragma once
#include <glm\glm.hpp>
#include <glm\vec2.hpp>
#include "Entities\GameObject.h"

#include "BehaviourController.h"

namespace aie {
	class Renderer2D;
}

class Behaviour {
public:
	Behaviour(const char* name = nullptr) : m_name(name) {
		m_id = ms_nextID++;
	}
	virtual ~Behaviour() = default;

	/** Perform actions upon activating the behaviour*/
	virtual void entryActions() = 0;
	/** Perform the behaviour's actions*/
	virtual void doActions(float deltaTime) = 0;
	/** Perform actions upon disabling the behaviour*/
	virtual void exitActions() = 0;
	/** Allow for debugging render of behaviours*/
	virtual void debugRender(aie::Renderer2D *renderer) = 0;

	/** Sets the speed the GameObject the component is attached to will accelerate at*/
	void setStrength(float val) { m_strength = val; }
	/** Get the speed the GameObject the component is attached is accelerating at*/
	const float getStrength() { return m_strength; }

	/** Set the amount of scaling applied to the behaviours strength*/
	void setPriorityWeight(float weight) { m_priorityWeight = weight; }
	/** Get the amount of scaling applied to the behaviours strength*/
	const float getPriorityWeight() { return m_priorityWeight; }

	/** Gets the scaling used to apply automatic weighting for steering behaviours*/
	float getForceScale() {
		// Handle automatic weighted steering behaviours
		float sumPriorityWeight = 0;
		for (auto behaviour : m_parentObject->getBehaviourController()->getBehaviours()) {
			sumPriorityWeight += behaviour.second->getPriorityWeight();
		}
		float forceScale = m_priorityWeight / sumPriorityWeight;
		return forceScale;
	}

	/** Sets the game object attached to the behaviour*/
	void setParent(GameObject *parent) { m_parentObject = parent; }

	/** Get the unique ID associated with the behaviour*/
	const unsigned int getID() { return m_id; }

	/** Get the name associated with the behaviour*/
	const char *getName() { return m_name; }


protected:
	GameObject *m_parentObject = nullptr;
	
	float m_strength = 0;

	float m_priorityWeight; // Amount to scale the force affecting the entity by

private:
	static unsigned int ms_nextID;
	unsigned int m_id;
	const char *m_name;
};