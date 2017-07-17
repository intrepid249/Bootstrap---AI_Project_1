#pragma once

class GameObject;
namespace aie {
	class Renderer2D;
}

class Behaviour {
protected:
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

protected:
	GameObject *m_parentObject = nullptr;
};