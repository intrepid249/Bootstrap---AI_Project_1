#include "Pathfinding\Pathfinder.h"
#include <algorithm>
#include <glm\vec2.hpp>
#include <Renderer2D.h>

Pathfinder::Pathfinder(Graph2D *graph) : m_pathFound(false), m_graph(graph) {
}

Pathfinder::~Pathfinder() {
	for (auto node : m_open)
		delete node;
	for (auto node : m_closed)
		delete node;

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
	algorithm = DIJKSTRA;

	m_isGoalNode = isGoalNodeCallback;

	Node *node = new Node();
	node->node = startNode;
	node->gScore = 0;
	node->parent = nullptr;

	m_open.push_back(node);

	m_pathFound = false;
}

void Pathfinder::findPath(Graph2D::Node * startNode, Graph2D::Node *endNode, std::function<float()> calcHeuristic) {
	algorithm = ASTAR;
	m_goalNode = endNode;

	m_calcHeuristic = calcHeuristic;

	Node *node = new Node();
	node->node = startNode;
	node->gScore = 0;
	node->parent = nullptr;

	m_open.push_back(node);

	m_pathFound = false;
}

bool Pathfinder::pathFound() {
	return m_pathFound;
}

void Pathfinder::updateSearch() {
	if (m_open.empty())
		m_pathFound = true;

	if (!m_pathFound) {
		Node * node = m_open.back();
		m_open.pop_back();
		m_closed.push_back(node);

		if (m_isGoalNode && m_isGoalNode(node->node)) {
			calculatePath(node);
			
			m_pathFound = true;
			return;
		} else if (algorithm == ASTAR && node->node == m_goalNode) {
			calculatePath(node);

			m_pathFound = true;
			return;

		}

		auto edges = node->node->edges;
		for (auto edge : edges) {
			auto child = edge.to;

			// Calculate cost of travelling to child node
			int cost = edge.data;
			float gScore = node->gScore + cost;

			if (algorithm == ASTAR && m_calcHeuristic)
				gScore += m_calcHeuristic();

			Node *nodeInList = getNodeInList(m_open, child);
			if (nodeInList == nullptr)
				nodeInList = getNodeInList(m_closed, child);

			if (nodeInList == nullptr) {
				Node *newNode = new Node();
				newNode->node = child;
				newNode->gScore = gScore;
				newNode->parent = node;

				m_open.push_back(newNode);
			} else {
				if (nodeInList->gScore > gScore) {
					nodeInList->parent = node;
					nodeInList->gScore = gScore;
				}
			}
		}

		m_open.sort([this](Node *a, Node *b) {
			return a->gScore > b->gScore;
		});
	}
}

Path & Pathfinder::getPath() {
	return m_path;
}

const int Pathfinder::getAlgorithm() {
	return algorithm;
}

Pathfinder::Node * Pathfinder::getNodeInList(std::list<Node*> list, Graph2D::Node * node) {
	for (auto iter = list.begin(); iter != list.end(); iter++) {
		if ((*iter)->node == node)
			return (*iter);
	}
	return nullptr;
}

void Pathfinder::calculatePath(Node * goal) {
	m_path.clear();
	Node *current = goal;
	while (current != nullptr) {
		m_path.addPathSegment(current->node->data);

		current = current->parent;
	}
}
