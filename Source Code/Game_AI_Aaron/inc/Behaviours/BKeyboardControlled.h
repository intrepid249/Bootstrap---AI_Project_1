#pragma once
#include "Behaviour.h"
#include <map>

namespace aie {
	enum EInputCodes;
}

class BKeyboardControlled : public Behaviour {
public:
	BKeyboardControlled();
	~BKeyboardControlled();

	/** Perform actions upon activating the behaviour*/
	virtual void entryActions();
	/** Perform the behaviour's actions*/
	virtual void doActions(float deltaTime);
	/** Perform actions upon disabling the behaviour*/
	virtual void exitActions();
	/** Allow for debugging render of behaviours*/
	virtual void debugRender(aie::Renderer2D *renderer);

	/** Sets the input buttons for the control layout*/
	void setControls(aie::EInputCodes upKey, aie::EInputCodes downKey, aie::EInputCodes leftKey, aie::EInputCodes rightKey);

private:
	void updateControls();

	enum eKeyID { LEFT, RIGHT, UP, DOWN };
	std::map<eKeyID, aie::EInputCodes> m_controls;

	bool m_moveUp, m_moveDown, m_moveLeft, m_moveRight;
};