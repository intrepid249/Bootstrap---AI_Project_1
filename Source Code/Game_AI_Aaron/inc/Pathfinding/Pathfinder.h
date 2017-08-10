#pragma once
#include "Graph\Graph2D.h"
#include "Graph\Path.h"

#include <functional>
#include <list>
#include <memory>

namespace aie {
	class Renderer2D;
}

class Pathfinder {
protected:
	struct Node {
		Graph2D::Node *node = nullptr;
		Node *parent = nullptr;

		float gScore;
	};

public:
	Pathfinder(Graph2D *graph = nullptr);
	~Pathfinder();

	void setGraph(Graph2D *graph);
	Graph2D *getGraph();

	/** Use djikstra algorithm to find a path*/
	void findPath(Graph2D::Node *startNode, std::function<bool(Graph2D::Node *)> isGoalNodeCallback);
	/** Use Astar algorithm to find a path*/
	void findPath(Graph2D::Node * startNode, Graph2D::Node *endNode, std::function<float()> calcHeuristic);

	bool pathFound();

	void updateSearch();

	Path& getPath();
	const int getAlgorithm();

protected:
	enum eAlgorithm { DIJKSTRA, ASTAR };

	Graph2D *m_graph;
	std::function<bool(Graph2D::Node *)> m_isGoalNode;
	std::function<float()> m_calcHeuristic;
	bool m_pathFound;
	Path m_path;

	std::list<Node *> m_open, m_closed;

private:
	Node *getNodeInList(std::list<Node *> list, Graph2D::Node *node);
	void calculatePath(Node *goal);

	int algorithm;
};