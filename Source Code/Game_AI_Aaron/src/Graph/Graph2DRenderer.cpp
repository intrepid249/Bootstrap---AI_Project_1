#include <Graph\Graph2DRenderer.h>
#include <Graph\Graph2D.h>
#include <Renderer2D.h>

Graph2DRenderer::Graph2DRenderer() : m_graph(nullptr) {
}

Graph2DRenderer::~Graph2DRenderer() {
}

void Graph2DRenderer::setGraph(Graph2D * graph) {
	m_graph = graph;
}

Graph2D * Graph2DRenderer::getGraph() {
	return m_graph;
}

void Graph2DRenderer::update(float deltaTime) {
}

void Graph2DRenderer::render(aie::Renderer2D * renderer) {
	auto &nodes = m_graph->getNodes();
	
	for (auto node : nodes) {
		renderer->drawCircle(node->data.x, node->data.y, 4);

		// Loop through all nodes and draw the edges
		for (auto edge : node->edges) {
			auto nodeB = edge.to;
			renderer->drawLine(node->data.x, node->data.y, nodeB->data.x, nodeB->data.y);
		}
	}

	
}
