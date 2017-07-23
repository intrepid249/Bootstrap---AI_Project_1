#pragma once
#include <glm\vec2.hpp>
#include <vector>

class Path {
public:
	Path();
	~Path();

	void addPathSegment(const glm::vec2& point);
	void popPathSegment();
	void clear();

	std::vector<glm::vec2>& getPath();

protected:
	std::vector<glm::vec2> m_pathPoints;

};