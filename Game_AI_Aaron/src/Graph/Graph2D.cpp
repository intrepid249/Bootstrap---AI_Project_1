#include <Graph\Graph2D.h>

void Graph2D::getNearbyNodes(glm::vec2 pos, float radius, std::vector<Node*>& nearbyNodes) {
	for (auto node : m_nodes) {
		if (node->data.x - pos.x <= radius && node->data.y - pos.y <= radius)
			nearbyNodes.push_back(node.get());
	}
}
