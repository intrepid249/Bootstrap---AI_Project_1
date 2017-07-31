#include "Behaviours\BKeyboardControlled.h"
#include "Entities\GameObject.h"

#include <Input.h>

BKeyboardControlled::BKeyboardControlled() : Behaviour() {
	m_moveDown = m_moveUp = m_moveLeft = m_moveRight = false;
	setControls(aie::INPUT_KEY_W, aie::INPUT_KEY_S, aie::INPUT_KEY_A, aie::INPUT_KEY_D);
}

BKeyboardControlled::~BKeyboardControlled() {
}

void BKeyboardControlled::entryActions() {
	// Reset the friction, in case of modification
	m_parentObject->setFriction(1);
}

void BKeyboardControlled::doActions(float deltaTime) {
	if (m_moveUp)
		m_parentObject->applyForce(glm::vec2(0, m_strength));
	if (m_moveDown)
		m_parentObject->applyForce(glm::vec2(0, -m_strength));
	if (m_moveLeft)
		m_parentObject->applyForce(glm::vec2(-m_strength, 0));
	if (m_moveRight)
		m_parentObject->applyForce(glm::vec2(m_strength, 0));

	updateControls();
}

void BKeyboardControlled::exitActions() {
}

void BKeyboardControlled::debugRender(aie::Renderer2D * renderer) {
}

void BKeyboardControlled::setControls(aie::EInputCodes upKey, aie::EInputCodes downKey, aie::EInputCodes leftKey, aie::EInputCodes rightKey) {
	m_controls[UP]	= upKey;
	m_controls[DOWN] = downKey;
	m_controls[LEFT] = leftKey;
	m_controls[RIGHT] = rightKey;
}

void BKeyboardControlled::updateControls() {
	aie::Input * input = aie::Input::getInstance();

	if (input->isKeyDown(m_controls[UP]))
		m_moveUp = true;
	if (input->isKeyDown(m_controls[DOWN]))
		m_moveDown = true;
	if (input->isKeyDown(m_controls[LEFT]))
		m_moveLeft = true;
	if (input->isKeyDown(m_controls[RIGHT]))
		m_moveRight = true;

	if (input->isKeyUp(m_controls[UP]))
		m_moveUp = false;
	if (input->isKeyUp(m_controls[DOWN]))
		m_moveDown = false;
	if (input->isKeyUp(m_controls[LEFT]))
		m_moveLeft = false;
	if (input->isKeyUp(m_controls[RIGHT]))
		m_moveRight = false;
}
