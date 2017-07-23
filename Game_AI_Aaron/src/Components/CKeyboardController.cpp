#include <Components\CKeyboardController.h>
#include <Input.h>
#include <glm\vec2.hpp>
#include <Entities\GameObject.h>

CKeyboardController::CKeyboardController() {
}

CKeyboardController::~CKeyboardController() {
}

void CKeyboardController::update(float deltaTime) {
	updateControls();

	if (m_moveUp)
		m_parent->applyForce(glm::vec2(0, m_movementSpeed));
	if (m_moveDown)
		m_parent->applyForce(glm::vec2(0, -m_movementSpeed));
	if (m_moveLeft)
		m_parent->applyForce(glm::vec2(-m_movementSpeed, 0));
	if (m_moveRight)
		m_parent->applyForce(glm::vec2(m_movementSpeed, 0));
}

void CKeyboardController::render(aie::Renderer2D * renderer) {
}

void CKeyboardController::setMovementSpeed(float val) {
	m_movementSpeed = val;
}

void CKeyboardController::updateControls() {
	aie::Input *input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_moveUp = true;
	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_moveDown = true;
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_moveLeft = true;
	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_moveRight = true;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_moveUp = false;
	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_moveDown = false;
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_moveLeft = false;
	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_moveRight = false;
}
