#pragma once
#include <glm\glm.hpp>
#include <glm\vec2.hpp>

class GameObject;
namespace aie {
	class Renderer2D;
}

class Behaviour {
public:
	Behaviour() = default;
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

	/** Sets the game object attached to the behaviour*/
	void setParent(GameObject *parent) { m_parentObject = parent; }

protected:
	GameObject *m_parentObject = nullptr;
	
	float m_strength = 0;

	float m_priorityWeight; // Amount to scale the force affecting the entity by
};