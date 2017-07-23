#include "Behaviours\BSeek.h"
#include "Entities\GameObject.h"

#include <Renderer2D.h>
#include <glm\glm.hpp>

BSeek::BSeek() : m_innerRadius(0), m_outerRadius(0) {

}

BSeek::~BSeek() {

}

void BSeek::entryActions() {
}

void BSeek::doActions(float deltaTime) {
	if (m_parentObject != nullptr) {
		float last_dist_to_target = glm::length(m_targetPos - m_lastPos);
		float dist_to_target = glm::length(m_targetPos - m_parentObject->getPos());

		if (m_onInnerRadiusEnter && last_dist_to_target > m_innerRadius && dist_to_target <= m_innerRadius)
			// If we have just entered the destination zone
			m_onInnerRadiusEnter();
		if (m_onInnerRadiusExit && last_dist_to_target <= m_innerRadius && dist_to_target > m_innerRadius)
			// If we have just left the destination zone
			m_onInnerRadiusExit();
		if (m_onOuterRadiusEnter && last_dist_to_target > m_outerRadius && dist_to_target <= m_outerRadius)
			// If we have just entered the agro zone
			m_onOuterRadiusEnter();
		if (m_onOuterRadiusExit && last_dist_to_target <= m_outerRadius && dist_to_target > m_outerRadius)
			// If we have just left the agro zone
			m_onOuterRadiusExit();



		glm::vec2 dir_to_target = glm::normalize(m_targetPos - m_parentObject->getPos()) * m_strength;

		m_parentObject->applyForce(dir_to_target);


		m_lastPos = m_parentObject->getPos();
	}
}

void BSeek::exitActions() {
}

void BSeek::debugRender(aie::Renderer2D * renderer) {
#ifdef _DEBUG
	renderer->drawBox(m_targetPos.x, m_targetPos.y, 4, 4);

	renderer->setRenderColour(1.0f, 1.0f, 1.0f, 0.5f);
	renderer->drawCircle(m_targetPos.x, m_targetPos.y, m_innerRadius);
	renderer->drawCircle(m_targetPos.x, m_targetPos.y, m_outerRadius);
	renderer->setRenderColour(1, 1, 1, 1);
#endif // _DEBUG
}

void BSeek::setTarget(const glm::vec2 & target) {
	m_targetPos = target;
}

const glm::vec2 & BSeek::getTarget() {
	return m_targetPos;
}

void BSeek::setInnerRadius(float radius) {
	m_innerRadius = radius;
}

float BSeek::getInnerRadius() {
	return m_innerRadius;
}

void BSeek::setOuterRadius(float radius) {
	m_outerRadius = radius;
}

float BSeek::getOuterRadius() {
	return m_outerRadius;
}

void BSeek::onInnerRadiusEnter(std::function<void()> callback) {
	m_onInnerRadiusEnter = callback;
}

void BSeek::onInnerRadiusExit(std::function<void()> callback) {
	m_onInnerRadiusExit = callback;
}

void BSeek::onOuterRadiusEnter(std::function<void()> callback) {
	m_onOuterRadiusEnter = callback;
}

void BSeek::onOuterRadiusExit(std::function<void()> callback) {
	m_onOuterRadiusExit = callback;
}
