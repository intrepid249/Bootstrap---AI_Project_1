#include "Pathfinding\Pathfinder.h"

Pathfinder::Pathfinder(Graph2D *graph) : m_pathFound(false), m_graph(graph) {
}

Pathfinder::~Pathfinder() {
	m_open.clear();
	m_closed.clear();
}

void Pathfinder::setGraph(Graph2D * graph) {
	m_graph = graph;
}

Graph2D * Pathfinder::getGraph() {
	return m_graph;
}

void Pathfinder::findPath(Graph2D::Node * startNode, std::function<bool(Graph2D::Node*)> isGoalNodeCallback) {
	m_isGoalNode = isGoalNodeCallback;

	std::unique_ptr<Node> node = std::unique_ptr<Node>(new Node());
	node->node = startNode;
	node->gScore = 0;
	node->parent = nullptr;

	m_open.push_back(std::move(node));

	m_pathFound = false;
}

bool Pathfinder::pathFound() {
	return m_pathFound;
}

void Pathfinder::updateSearch() {
	if (m_open.empty())
		m_pathFound = true;

	if (!m_pathFound) {
		std::unique_ptr<Node> node = std::move(m_open.back());
		m_open.pop_back();
		m_closed.push_back(node);

		if (m_isGoalNode && m_isGoalNode(node->node)) {
			// TODO: calculate path
			m_pathFound = true;
			return;
		}

		auto edges = node->node->edges;
		for (auto edge : edges) {
			auto child = edge.to;

			// Calculate cost of travelling to child node
			int cost = edge.data;
			float gScore = node->gScore + cost;

			// TypeHere
		}
	}
}

Path & Pathfinder::getPath() {
	return m_path;
}
