#include <Graph\Graph2D.h>
#include <glm\glm.hpp>

void Graph2D::getNearbyNodes(const glm::vec2 &pos, float radius, std::vector<Node*>& nearbyNodes) {
	for (auto node : m_nodes) {
		if (glm::length(pos - node->data) < radius)
			nearbyNodes.push_back(node.get());
	}
}
