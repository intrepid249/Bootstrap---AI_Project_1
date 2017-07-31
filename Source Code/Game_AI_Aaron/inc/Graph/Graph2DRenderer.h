#pragma once
#include <glm\vec2.hpp>

class Graph2D;
namespace aie {
	class Renderer2D;
}

class Graph2DRenderer {
public:
	Graph2DRenderer();
	~Graph2DRenderer();

	void setGraph(Graph2D *graph);
	Graph2D *getGraph();

	void update(float deltaTime);
	void render(aie::Renderer2D *renderer);

private:
	Graph2D *m_graph;

};