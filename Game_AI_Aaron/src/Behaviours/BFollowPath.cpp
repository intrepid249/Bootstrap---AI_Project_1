#include "Behaviours\BFollowPath.h"
#include "Graph\Path.h"
#include "Entities\GameObject.h"
#include "Utilities\Colour.h"

#include <Renderer2D.h>
#include <glm\glm.hpp>

BFollowPath::BFollowPath() : Behaviour(), m_path(nullptr), m_currentPathNodeIndex(0), m_nodeRadius(10), m_patrolDir(1) {
}

BFollowPath::~BFollowPath() {
}

void BFollowPath::entryActions() {
}

void BFollowPath::doActions(float deltaTime) {
	auto &path = m_path->getPath();
	if (!path.empty()) {
		m_currentPathNodeIndex = m_currentPathNodeIndex % path.size();
		glm::vec2 point = path[m_currentPathNodeIndex];

		float last_dist_to_target = glm::length(point - m_lastPos);
		float dist_to_target = glm::length(point - m_parentObject->getPos());
		if ((m_patrolDir > 0 && m_currentPathNodeIndex == path.size() - 1) || (m_patrolDir < 0 && m_currentPathNodeIndex == 0)) {
			// If we are targeting the last node - for both directions

			if (m_onLastNodeReached && last_dist_to_target > getNodeRadius() && dist_to_target <= getNodeRadius())
				m_onLastNodeReached();
		}

		if (glm::length(point - m_parentObject->getPos()) < getNodeRadius())
			m_currentPathNodeIndex += m_patrolDir;

		// Seek toward the point
		glm::vec2 dir_to_point = glm::normalize(point - m_parentObject->getPos()) * m_strength;
		m_parentObject->applyForce(dir_to_point);

		m_lastPos = m_parentObject->getPos();
	}
}

void BFollowPath::exitActions() {
}

void BFollowPath::debugRender(aie::Renderer2D * renderer) {
#ifdef _DEBUG
	auto &path = m_path->getPath();
	glm::vec2 lastPathPoint;
	int index = 0;
	for (auto node : path) {
		renderer->setRenderColour(1, 1, 1, 0.5f);
		renderer->drawCircle(node.x, node.y, getNodeRadius());
		renderer->setRenderColour(1, 1, 1, 1);

		Colour c = (index == m_currentPathNodeIndex) ? 0xff0a4fff : 0xffffffff;
		renderer->setRenderColour(c);

		renderer->drawCircle(node.x, node.y, 4);
		
		if (index > 0)
			renderer->drawLine(node.x, node.y, lastPathPoint.x, lastPathPoint.y);
		renderer->setRenderColour(1, 1, 1, 1);

		lastPathPoint = node;
		index++;
	}
#endif // _DEBUG
}

void BFollowPath::setPath(Path * path) {
	m_path = path;
}

Path * BFollowPath::getPath() {
	return m_path;
}

void BFollowPath::setNodeRadius(float radius) {
	m_nodeRadius = radius;
}

float BFollowPath::getNodeRadius() {
	return m_nodeRadius;
}

void BFollowPath::isPatrolling(bool flag) {
	m_isPatrolling = flag;
}

bool BFollowPath::isPatrolling() {
	return m_isPatrolling;
}

void BFollowPath::setPatrolDir(int increment) {
	m_patrolDir = increment;
}

int BFollowPath::getPatrolDir() {
	return m_patrolDir;
}

void BFollowPath::onLastNodeReached(std::function<void()> callback) {
	m_onLastNodeReached = callback;
}
