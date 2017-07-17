#include <Entities\GameObject.h>
#include <Renderer2D.h>

GameObject::GameObject() : m_friction(0.0f) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
	applyForce(m_friction * -m_velocity);

	m_velocity += m_acceleration * deltaTime;
	m_pos += m_velocity * deltaTime;
	m_acceleration = glm::vec2();
}

void GameObject::render(aie::Renderer2D * renderer) {
	renderer->drawCircle(m_pos.x, m_pos.y, 8);

	// Draw the heading/velocity
	renderer->setRenderColour(0xFF7F00FF);
	glm::vec2 targetHeading = m_pos + m_velocity;
	renderer->drawLine(m_pos.x, m_pos.y, targetHeading.x, targetHeading.y, 2.f);
	renderer->setRenderColour(0xFFFFFFFF);
}

void GameObject::applyForce(const glm::vec2 & _force) {
	m_acceleration += _force;
}

#pragma region Getters and Setters
void GameObject::setPos(const glm::vec2& _pos) {
	m_pos = _pos;
}

const glm::vec2 & GameObject::getPos() {
	return m_pos;
}

void GameObject::setVelocity(const glm::vec2& _velocity) {
	m_velocity = _velocity;
}

const glm::vec2 & GameObject::getVelocity() {
	return m_velocity;
}

void GameObject::setFriction(const float & _friction) {
	m_friction = _friction;
}

const float & GameObject::getFriction() {
	return m_friction;
}
#pragma endregion

#pragma region Screen Functions
void GameObject::wrapScreenBounds() {
	ini_t cfg("config/settings.ini", true);
	int winWidth = cfg.get("DisplayOptions", "WindowWidth", 1);
	int winHeight = cfg.get("DisplayOptions", "WindowHeight", 1);

	if (m_pos.x < 0) setPos(glm::vec2(winWidth, m_pos.y));
	if (m_pos.x > winWidth) setPos(glm::vec2(0, m_pos.y));
	if (m_pos.y < 0) setPos(glm::vec2(winHeight, m_pos.x));
	if (m_pos.y > winHeight) setPos(glm::vec2(0, m_pos.x));
}

void GameObject::constrainToScreenBounds() {
	ini_t cfg("config/settings.ini", true);
	int winWidth = cfg.get("DisplayOptions", "WindowWidth", 1);
	int winHeight = cfg.get("DisplayOptions", "WindowHeight", 1);

	
}

void GameObject::destroyOnExitScreen() {
}
#pragma endregion