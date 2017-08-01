#include "Behaviours\BWander.h"
#include "Behaviours\BSeek.h"
#include "Entities\GameObject.h"

#include "GlobalConfig.h"

#include <jm_utilities.h>

#include <glm\glm.hpp>
#include <random>
#include <Renderer2D.h>

BWander::BWander() {
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

		static float directionTimer = m_newDirDelay;
		static float randomAngle;

		directionTimer += deltaTime;
		if (directionTimer < m_newDirDelay) return;

		// Get the direction of travel and project a distance in front of the object
		glm::vec2 projectionVector;

		if (glm::length(m_parentObject->getVelocity()) == 0)
			projectionVector = glm::normalize(glm::vec2(cosf(m_parentObject->getRotation()), sinf(m_parentObject->getRotation()))) * m_projectionDistance;
		else
			projectionVector = glm::normalize(m_parentObject->getVelocity()) * m_projectionDistance;

		// Use the radius and the projectedVector in order to construct a circle and use polar coordinates
		// to determine a new heading
		randomAngle = (float)(rand() * 360); // get a bearing
		// Convert polar coordinate system to the cartesian plane for use
		glm::vec2 newTarget = glm::vec2(cosf(randomAngle) * m_radius, sinf(randomAngle) * m_radius) + projectionVector;

		// Apply a force towards the new position
		m_seeker->setTarget(newTarget);
		m_seeker->doActions(deltaTime);

		directionTimer = 0;
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
