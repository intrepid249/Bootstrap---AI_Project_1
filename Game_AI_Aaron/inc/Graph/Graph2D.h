#pragma once
#include <Graph.hpp>
#include <glm\vec2.hpp>
#include <vector>

class Graph2D : public Graph<glm::vec2, int> {
public:
	Graph2D() = default;
	virtual ~Graph2D() = default;

	void getNearbyNodes(const glm::vec2& pos, float radius, std::vector<Node*> &nearbyNodes);
};