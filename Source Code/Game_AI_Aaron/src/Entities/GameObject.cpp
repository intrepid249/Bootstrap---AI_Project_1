#include "Entities\GameObject.h"

#include "GlobalConfig.h"
#include "Utilities\jm_utilities.h"
#include "Components\JM_Component.h"
#include "Behaviours\Behaviour.h"

#include <Renderer2D.h>
#include <glm\glm.hpp>
#include <Texture.h>

GameObject::GameObject(aie::Texture *tex) : m_tex(tex), m_friction(0.0f), m_drawn(true) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
	if (isDrawn()) {
		applyForce(m_friction * -m_velocity);

		m_velocity += m_acceleration * deltaTime;
		//truncate(m_velocity, maxSpeed);

		m_pos += m_velocity * deltaTime;
		m_acceleration = glm::vec2();

		for (size_t i = 0; i < m_components.size(); ++i)
			m_components[i]->update(deltaTime);

		if (m_behaviour != nullptr)
			m_behaviour->doActions(deltaTime);
	}
}

void GameObject::render(aie::Renderer2D * renderer) {
	if (isDrawn()) {
		glm::vec2 targetHeading = m_pos + m_velocity;
		float rot = atan2f(targetHeading.y - m_pos.y, targetHeading.x - m_pos.x);

		if (m_tex != nullptr)
			renderer->drawSprite(m_tex, m_pos.x, m_pos.y, 0, 0, rot);
		else
			renderer->drawBox(m_pos.x, m_pos.y, 10, 10, 0.2f);

#ifdef _DEBUG
		// Draw a dot at the object's position
		renderer->drawCircle(m_pos.x, m_pos.y, 3);

		// Draw the heading/velocity
		renderer->setRenderColour(0xFF7F00FF);
		renderer->drawLine(m_pos.x, m_pos.y, targetHeading.x, targetHeading.y, 2.f);
		renderer->setRenderColour(0xFFFFFFFF);

		// Draw the lookat rotation - used to rotate sprites properly according to movement
		renderer->setRenderColour(0xFF007FFF);
		renderer->drawCircle(m_pos.x + cosf(rot) * 20, m_pos.y + sinf(rot) * 20, 2);
		renderer->setRenderColour(0xFFFFFFFF);

		for (size_t i = 0; i < m_components.size(); ++i)
			m_components[i]->render(renderer);

		if (m_behaviour != nullptr)
			m_behaviour->debugRender(renderer);
#endif // _DEBUG
	}
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
void GameObject::addComponent(std::shared_ptr<JM_Component> _component) {
	m_components.push_back(std::move(_component));
}
void GameObject::setDraw(bool val) {
	m_drawn = val;
}
bool GameObject::isDrawn() {
	return m_drawn;
}
void GameObject::setBehaviour(std::shared_ptr<Behaviour> behaviour) {
	if (m_behaviour)
		m_behaviour->exitActions();

	m_behaviour = behaviour;
	if (m_behaviour)
		m_behaviour->entryActions();
}
Behaviour * GameObject::getBehaviour() {
	return m_behaviour.get();
}
#pragma endregion

#pragma region Screen Functions
void GameObject::wrapScreenBounds() {
	ini_t *cfg = GlobalConfig::getInstance();
	int winWidth = cfg->get("DisplayOptions", "WindowWidth", 1);
	int winHeight = cfg->get("DisplayOptions", "WindowHeight", 1);

	if (m_pos.x < 0) setPos(glm::vec2(winWidth, m_pos.y));
	if (m_pos.x > winWidth) setPos(glm::vec2(0, m_pos.y));
	if (m_pos.y < 0) setPos(glm::vec2(winHeight, m_pos.x));
	if (m_pos.y > winHeight) setPos(glm::vec2(0, m_pos.x));
}

void GameObject::constrainToScreenBounds(bool bounce) {
	ini_t *cfg = GlobalConfig::getInstance();
	float winWidth = cfg->get("DisplayOptions", "WindowWidth", float());
	float winHeight = cfg->get("DisplayOptions", "WindowHeight", float());
	float width = (m_tex != nullptr) ? (float)m_tex->getWidth() : 0;
	float height = (m_tex != nullptr) ? (float)m_tex->getHeight() : 0;

	bool hitLeft = (m_pos.x - width / 2 < 0), hitRight = (m_pos.x + width / 2 > winWidth);
	bool hitBottom = (m_pos.y - height / 2 < 0), hitTop = (m_pos.y + height / 2 > winHeight);

	if (bounce) {
		if (hitLeft || hitRight) {
			setPos(glm::vec2(winWidth - width / 2, m_pos.y));
			applyForce(glm::vec2(-(m_velocity.x * REFLECTION_FORCE), 0));
		}
	} else {

	}
}

void GameObject::destroyOnExitScreen() {
}
#pragma endregion