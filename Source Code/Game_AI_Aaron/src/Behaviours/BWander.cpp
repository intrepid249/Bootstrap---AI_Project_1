#include "Behaviours\BWander.h"
#include "Entities\GameObject.h"

#include <glm\glm.hpp>
#include <random>
#include <Renderer2D.h>

BWander::BWander() {
}

BWander::~BWander() {
}

void BWander::entryActions() {
}

void BWander::doActions(float deltaTime) {
	static float directionTimer = m_newDirDelay;

	directionTimer += deltaTime;
	if (directionTimer < m_newDirDelay) return;

	// Get the direction of travel and project a distance in front of the object
	glm::vec2 projectionVector = glm::normalize(m_parentObject->getVelocity()) + m_projectionDistance;

	// Use the radius and the projectedVector in order to construct a circle and use polar coordinates
	// to determine a new heading
	float randomAngle = (rand() % 360); // get a bearing
	// Convert polar coordinate system to the cartesian plane for use
	glm::vec2 newTarget = glm::vec2(cosf(randomAngle) * m_radius, sinf(randomAngle) * m_radius) - projectionVector;

	// Calculate a new velocity
	glm::vec2 desired_velocity = (newTarget - m_parentObject->getPos()) * m_strength;

	// Apply a force towards the new position
	//m_parentObject->applyForce(desired_velocity);
	directionTimer = 0;
}

void BWander::exitActions() {
}

void BWander::debugRender(aie::Renderer2D * renderer) {
	glm::vec2 projectionVector = glm::normalize(m_parentObject->getVelocity()) + m_projectionDistance;

	renderer->setRenderColour(1, 0.6f, 0, 1.f);
	renderer->drawLine(m_parentObject->getPos().x, m_parentObject->getPos().y, projectionVector.x, projectionVector.y);
	renderer->setRenderColour(1, 1, 1, 1);
}

void BWander::setProjectionDistance(const float val) {
	m_projectionDistance = val;
}

float BWander::getProjectionDistance() const {
	return m_projectionDistance;
}

void BWander::setRadius(const float val) {
	m_radius = val;
}

float BWander::getRadius() const {
	return m_radius;
}