#include "Graph\Path.h"

Path::Path() {
}

Path::~Path() {
}

void Path::addPathSegment(const glm::vec2 & point) {
	m_pathPoints.push_back(point);
}

void Path::popPathSegment() {
	if (!m_pathPoints.empty())
		m_pathPoints.pop_back();
}

void Path::clear() {
	m_pathPoints.clear();
}

std::vector<glm::vec2>& Path::getPath() {
	return m_pathPoints;
}
