#pragma once
#include "Application.h"
#include <memory>
#include <Graph\Graph2D.h>
#include <Graph\Graph2DRenderer.h>

#include "Entities\Player.h"


namespace aie {
	class Renderer2D;
	class Font;
}
class GameObject;

class Game_AI_AaronApp : public aie::Application {
public:

	Game_AI_AaronApp();
	virtual ~Game_AI_AaronApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void drawGraph();
	void generateGraph();
	bool generateGraphFromFile();
	void generateGraphEdges();
	void updateGraph(float deltaTime);

protected:
	std::shared_ptr<aie::Renderer2D>	m_renderer;

	std::unique_ptr<Player> m_player = nullptr;

	std::unique_ptr<Graph2D> m_graph;
	std::unique_ptr<Graph2DRenderer> m_graphRenderer;

	int m_graphSpacing;
	glm::vec2 m_mousePos;
};