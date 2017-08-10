#include "Entities\GameObject.h"

#include "GlobalConfig.h"
#include "Components\JM_Component.h"
#include "Behaviours\Behaviour.h"
#include "Behaviours\BehaviourController.h"
#include "Tileset.h"

#include "Graph\Path.h"
#include "Pathfinding\Pathfinder.h"

#include <Renderer2D.h>
#include <glm\glm.hpp>
#include <Texture.h>

GameObject::GameObject(aie::Texture *tex) : m_tex(tex), m_friction(0.0f), m_rotation(0), m_drawn(true), m_scaleAmount(glm::vec2(1, 1)),
m_size(glm::vec2(tex->getWidth(), tex->getHeight())), m_startNode(nullptr), m_endNode(nullptr) {

	m_behaviourController = std::shared_ptr<BehaviourController>(new BehaviourController());

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

		// Update all behaviours
		m_behaviourController->update(deltaTime);
	}
}

void GameObject::render(aie::Renderer2D * renderer, float depth) {
	if (isDrawn()) {
		glm::vec2 targetHeading = m_pos + m_velocity;
		m_rotation = atan2f(targetHeading.y - m_pos.y, targetHeading.x - m_pos.x);

		if (m_tex != nullptr)
			renderer->drawSprite(m_tex, m_pos.x, m_pos.y, m_tex->getWidth() * m_scaleAmount.x, m_tex->getHeight() * m_scaleAmount.y,
				m_rotation, depth);
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
		renderer->drawCircle(m_pos.x + cosf(m_rotation) * 20, m_pos.y + sinf(m_rotation) * 20, 2);
		renderer->setRenderColour(0xFFFFFFFF);

		for (size_t i = 0; i < m_components.size(); ++i)
			m_components[i]->render(renderer);

		// Perform debug rendering (if any) for all behaviours
		m_behaviourController->render(renderer);
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

void GameObject::scale(const float _scale) {
	m_scaleAmount = glm::vec2(_scale, _scale);
}

void GameObject::scale(const glm::vec2 & _scale) {
	m_scaleAmount = _scale;
}

const glm::vec2 GameObject::getSize() {
	return m_size * m_scaleAmount;
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

void GameObject::setGraph(Graph2D * graph) {
	m_graph = graph;
}

Graph2D * GameObject::getGraph() {
	return m_graph;
}

void GameObject::addComponent(std::shared_ptr<JM_Component> _component) {
	m_components.push_back(std::move(_component));
}
void GameObject::setDraw(bool flag) {
	m_drawn = flag;
}
bool GameObject::isDrawn() {
	return m_drawn;
}

void GameObject::setRotation(float angle) {
	m_rotation = angle;
}
float GameObject::getRotation() {
	return m_rotation;
}
const std::shared_ptr<BehaviourController>& GameObject::getBehaviourController() {
	return m_behaviourController;
}
#pragma endregion

#pragma region Screen Functions
void GameObject::checkCollisions(const std::vector<jm::Object>& objList) {
	float width = (m_tex != nullptr) ? getSize().x : 0;
	float height = (m_tex != nullptr) ? getSize().x : 0;

	for (auto iter = objList.begin(); iter != objList.end(); iter++) {
		jm::Object obj = (*iter);

		bool hitLeft = (m_pos.x - width / 2 < obj.x + obj.width / 2), hitRight = (m_pos.x + width / 2 > obj.x - obj.width / 2);
		bool hitBottom = (m_pos.y - height / 2 < obj.y + obj.height / 2), hitTop = (m_pos.y + width / 2 > obj.y - obj.height / 2);

		if (hitLeft)
			setPos(glm::vec2(m_pos.x + width / 2, m_pos.y));
		if (hitRight)
			setPos(glm::vec2(m_pos.x - width / 2, m_pos.y));
		if (hitBottom)
			setPos(glm::vec2(m_pos.x, m_pos.y + width / 2));
		if (hitTop)
			setPos(glm::vec2(m_pos.x, m_pos.y - width / 2));
	}
}

void GameObject::wrapScreenBounds(glm::vec2 cameraPos = glm::vec2(0)) {
	ini_t *cfg = GlobalConfig::getInstance();
	int winWidth = cfg->get("DisplayOptions", "WindowWidth", 1);
	int winHeight = cfg->get("DisplayOptions", "WindowHeight", 1);
	float width = (m_tex != nullptr) ? getSize().x : 0;
	float height = (m_tex != nullptr) ? getSize().x : 0;

	// TODO: Adjust for camera movement
	if (m_pos.x < 0) setPos(glm::vec2(winWidth, m_pos.y));
	if (m_pos.x > winWidth) setPos(glm::vec2(0, m_pos.y));
	if (m_pos.y < 0) setPos(glm::vec2(winHeight, m_pos.x));
	if (m_pos.y > winHeight) setPos(glm::vec2(0, m_pos.x));
}

void GameObject::constrainToScreenBounds(bool bounce, glm::vec2 cameraPos) {
	ini_t *cfg = GlobalConfig::getInstance();
	float winWidth = cfg->get("DisplayOptions", "WindowWidth", float());
	float winHeight = cfg->get("DisplayOptions", "WindowHeight", float());
	float width = (m_tex != nullptr) ? getSize().x : 0;
	float height = (m_tex != nullptr) ? getSize().x : 0;

	bool hitLeft = ((m_pos.x - width / 2) - cameraPos.x < 0), hitRight = ((m_pos.x + width / 2) - cameraPos.x > winWidth);
	bool hitBottom = ((m_pos.y - height / 2) - cameraPos.y < 0), hitTop = ((m_pos.y + height / 2) - cameraPos.y > winHeight);

	if (bounce) {
		if (hitLeft || hitRight) {
			setPos(glm::vec2(winWidth - width / 2, m_pos.y));
			applyForce(glm::vec2(-(m_velocity.x * REFLECTION_FORCE), 0));
		}
	} else {
		if (hitLeft)
			setPos(glm::vec2(width / 2, m_pos.y));
		else if (hitRight)
			setPos(glm::vec2(winWidth - width / 2, m_pos.y));
		if (hitBottom)
			setPos(glm::vec2(m_pos.x, height / 2));
		else if (hitTop)
			setPos(glm::vec2(m_pos.x, winHeight - height / 2));
	}
}

void GameObject::destroyOnExitScreen(glm::vec2 cameraPos = glm::vec2(0)) {
}
#pragma endregion