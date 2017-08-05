#include "Behaviours\BWander.h"
#include "Behaviours\BSeek.h"
#include "Entities\GameObject.h"

#include "GlobalConfig.h"

#include <jm_utilities.h>

#include <glm\glm.hpp>
#include <random>
#include <Renderer2D.h>

BWander::BWander() : m_newRotation(0), m_projectionDistance(10), m_radius(20) {
	m_seeker = std::shared_ptr<BSeek>(new BSeek());
	m_seeker->setStrength(PLAYER_MOVEMENT_SPEED);
}

BWander::~BWander() {
}

void BWander::entryActions() {
}

void BWander::doActions(float deltaTime) {
	if (m_parentObject != nullptr) {
		m_seeker->setParent(m_parentObject);

		// Get the direction of travel and project a distance in front of the object
		m_circleCentre = glm::normalize(glm::vec2(cosf(m_parentObject->getRotation()), sinf(m_parentObject->getRotation()))) * m_projectionDistance;

		// Use the radius and the projectedVector in order to construct a circle and use polar coordinates
		// to determine a new heading
		float prevAngle = m_newRotation;
		m_newRotation = prevAngle + (float)((rand() % 314) - 158) / 100.f;
		// Convert polar coordinate system to the cartesian plane for use
		glm::vec2 targetForce = glm::vec2(cosf(m_newRotation) * m_radius, sinf(m_newRotation) * m_radius) + m_circleCentre;

		// Apply a force towards the new position
		m_parentObject->applyForce(targetForce);
	}
}

void BWander::exitActions() {
}

void BWander::debugRender(aie::Renderer2D * renderer) {
	glm::vec2 projectionVector = glm::normalize(m_parentObject->getVelocity()) + m_projectionDistance;

	renderer->setRenderColour(1, 0.6f, 0, 1.f);
	//renderer->drawLine(m_parentObject->getPos().x, m_parentObject->getPos().y, projectionVector.x, projectionVector.y);
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
