#pragma once
#include <vector>
#include <memory>

template <typename TNodeData, typename TEdgeData>
class Graph {
public:
	struct Node;
	struct Edge;

	struct Node {
		TNodeData data;
		std::vector<Edge> edges;
	};

	struct Edge {
		Node *to;
		TEdgeData data;
	};

	int addNode(const TNodeData &data) {
		std::shared_ptr<Node> node = std::shared_ptr<Node>(new Node());
		node->data = data;

		m_nodes.push_back(std::move(node));
		return m_nodes.size() - 1;
	}

	void addEdge(int fromIndex, int toIndex, bool biDirectional, const TEdgeData &data) {
		Node *nodeA = getNodeByIdx(fromIndex);
		Node *nodeB = getNodeByIdx(toIndex);

		addEdge(nodeA, nodeB, biDirectional, data);
	}

	void addEdge(Node *fromNode, Node *toNode, bool biDirectional, const TEdgeData &data) {
		Edge edge1 = Edge();
		edge1.to = toNode;
		edge1.data = data;

		fromNode->edges.push_back(edge1);

		if (biDirectional) {
			Edge edge2 = Edge();
			edge2.to = fromNode;
			edge2.data = data;
			toNode->edges.push_back(edge2);
		}
	}

	Node *getNodeByIdx(int nodeID) {
		return m_nodes[nodeID].get();
	}

	const std::vector<std::shared_ptr<Node>> &getNodes() const {
		return m_nodes;
	}

	bool isCyclic() {}
	bool isConnected() {}

protected:
	std::vector<std::shared_ptr<Node>> m_nodes;
};