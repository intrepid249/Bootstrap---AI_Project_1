#include "Behaviours\BCollisionAvoidance.h"

#include <jm_utilities.h>
#include <jm_shapes.h>

#include <Renderer2D.h>

BCollisionAvoidance::BCollisionAvoidance() {
}

BCollisionAvoidance::~BCollisionAvoidance() {
}

void BCollisionAvoidance::entryActions() {
}

void BCollisionAvoidance::doActions(float deltaTime) {
	if (m_parentObject != nullptr) {
		// Create the "Viewing cone" that we can use to detect potential collisions ahead of, and to either side of the Parent object
		m_ahead = glm::normalize(glm::vec2(cosf(m_parentObject->getRotation()), sinf(m_parentObject->getRotation()))) * m_visionDistance;
		m_aheadLeft = glm::normalize(glm::vec2(cosf(m_parentObject->getRotation() + m_fov), sinf(m_parentObject->getRotation() + m_fov))) * m_visionDistance;
		m_aheadRight = glm::normalize(glm::vec2(cosf(m_parentObject->getRotation() - m_fov), sinf(m_parentObject->getRotation() - m_fov))) * m_visionDistance;

		glm::vec2 targetVelocity = glm::vec2(1);

		// Cast out rays to check for potential collision
		for (auto iter = m_collidableObjects.begin(); iter != m_collidableObjects.end(); iter++) {
			jm::Rect obj = (*iter);
			glm::vec2 objCentre = glm::vec2(obj.x + obj.width / 2, obj.y + obj.height / 2);

			// Cast the point back to the rays' origin to perform dot product and check intersection
			glm::vec2 pointCast = objCentre - m_parentObject->getPos();
			float dotAhead = jm::clampf(glm::dot(pointCast, m_ahead), 0, m_visionDistance);
			float dotLeft = jm::clampf(glm::dot(pointCast, m_aheadLeft), 0, m_visionDistance);
			float dotRight = jm::clampf(glm::dot(pointCast, m_aheadRight), 0, m_visionDistance);

			if (dotAhead > 0) {
				glm::vec2 closestPoint = glm::normalize(m_ahead) * dotAhead + m_parentObject->getPos();
				if (obj.containsPoint(closestPoint.x, closestPoint.y))
					targetVelocity = (m_ahead + m_parentObject->getPos()) - objCentre;
			} else if (dotLeft > 0) {
				glm::vec2 closestPoint = glm::normalize(m_aheadLeft) * dotLeft + m_parentObject->getPos();
				if (obj.containsPoint(m_parentObject->getPos().x + m_aheadLeft.x, m_parentObject->getPos().y + m_aheadLeft.y))
					targetVelocity = (m_aheadLeft + m_parentObject->getPos()) - objCentre;
			} else if (dotRight > 0) {
				glm::vec2 closestPoint = glm::normalize(m_aheadRight) * dotRight + m_parentObject->getPos();
				if (obj.containsPoint(m_aheadRight.x, m_aheadRight.y))
					targetVelocity = (m_aheadRight + m_parentObject->getPos()) - objCentre;
			}

		}

		if (glm::length(targetVelocity) > 0) {
			glm::vec2 newForce = glm::normalize(targetVelocity) * m_strength * getForceScale();
			m_parentObject->applyForce(newForce);
		}

	}
}

void BCollisionAvoidance::exitActions() {
}

void BCollisionAvoidance::debugRender(aie::Renderer2D * renderer) {

	renderer->setRenderColour(226 / 255.f, 213 / 255.f, 0.f, 1.f);
	renderer->drawLine(m_parentObject->getPos().x, m_parentObject->getPos().y, m_parentObject->getPos().x + m_ahead.x, m_parentObject->getPos().y + m_ahead.y);
	renderer->drawLine(m_parentObject->getPos().x, m_parentObject->getPos().y, m_parentObject->getPos().x + m_aheadLeft.x, m_parentObject->getPos().y + m_aheadLeft.y);
	renderer->drawLine(m_parentObject->getPos().x, m_parentObject->getPos().y, m_parentObject->getPos().x + m_aheadRight.x, m_parentObject->getPos().y + m_aheadRight.y);
	renderer->setRenderColour(1, 1, 1, 1);
}

void BCollisionAvoidance::setVisionDistance(float val) {
	m_visionDistance = val;
}

const float & BCollisionAvoidance::getVisionDistance() {
	return m_visionDistance;
}

void BCollisionAvoidance::setFOV(float fov) {
	m_fov = fov;
}

const float & BCollisionAvoidance::getFOV() {
	return m_fov;
}

void BCollisionAvoidance::setCollidableObjects(std::vector<jm::Rect> colliders) {
	m_collidableObjects = colliders;
}

//const jm::Rect & BCollisionAvoidance::findMostThreateningObstacle() {
//	for (auto iter = m_collidableObjects.begin(); iter != m_collidableObjects.end(); iter++) {
//		jm::Rect obstacle = (*iter);
//		// Convert the object position to centre anchor, as object coordinates are stored in the top-left
//		glm::vec2 objCentre = glm::vec2(obstacle.x + obstacle.width / 2, obstacle.y + obstacle.height / 2);
//		
//		
//	}
//}
