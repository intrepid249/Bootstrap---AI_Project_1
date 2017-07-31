#pragma once
#include "JM_Component.h"

class CKeyboardController : public JM_Component {
public:
	CKeyboardController();
	virtual ~CKeyboardController();

	/** Update members and actions each frame*/
	virtual void update(float deltaTime);
	/** Allow for displaying objects to the screen*/
	virtual void render(aie::Renderer2D *renderer);

	/** Sets the speed the GameObject the component is attached to will move at*/
	void setMovementSpeed(float val);

protected:
	void updateControls();

	bool m_moveUp, m_moveDown, m_moveLeft, m_moveRight;
	float m_movementSpeed;
};