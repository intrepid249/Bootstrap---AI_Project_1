#include "Game_AI_AaronApp.h"
#include <Renderer2D.h>
#include <Texture.h>
#include <Font.h>
#include <Input.h>
#include <Entities\GameObject.h>
#include <Graph\Graph2D.h>

#include <glm\glm.hpp>

Game_AI_AaronApp::Game_AI_AaronApp() {

}

Game_AI_AaronApp::~Game_AI_AaronApp() {

}

bool Game_AI_AaronApp::startup() {
	
	m_renderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_player = std::unique_ptr<GameObject>(new GameObject());
	m_player->setPos(glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));

	/// Graph
	m_graph = std::unique_ptr<Graph2D>(new Graph2D());
	const int numRows = 2, numCols = 2;
	const int spacing = 100;
	const int xOffset = 100, yOffset = 100;

	// Populate the graph with data
	for (int y = 0; y < numRows; y++) {
		for (int x = 0; x < numCols; x++) {
			m_graph->addNode(glm::vec2(x * spacing + xOffset, y * spacing + yOffset));
		}
	}

	// Connect all nearby nodes
	auto &nodes = m_graph->getNodes();
	for (auto iter1 = nodes.begin(); iter1 != nodes.end(); iter1++) {
		Graph2D::Node *nodeA = (*iter1).get();
		std::vector<Graph2D::Node*> nearbyNodes;
		m_graph->getNearbyNodes(nodeA->data, 150.f, nearbyNodes);

		// Loop through nearby nodes
		for (auto iter2 = nearbyNodes.begin(); iter2 != nearbyNodes.end(); iter2++) {
			Graph2D::Node *nodeB = (*iter2);
			if (nodeA == nodeB)
				continue; // Don't connect a node to itself

			int distBetweenNodes = (int)glm::length(nodeB->data - nodeA->data);
			m_graph->addEdge(nodeA, nodeB, true, distBetweenNodes);
		}
	}

	/// Draw graph
	m_graphRenderer = std::unique_ptr<Graph2DRenderer>(new Graph2DRenderer());
	m_graphRenderer->setGraph(m_graph.get());
	return true;
}

void Game_AI_AaronApp::shutdown() {
	delete m_font;
	delete m_renderer;
}

void Game_AI_AaronApp::update(float deltaTime) {

	m_player->applyForce(glm::vec2(10, 0));
	m_player->update(deltaTime);
	m_player->wrapScreenBounds();

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Game_AI_AaronApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_renderer->begin();

	//m_player->render(m_renderer);

	m_graphRenderer->render(m_renderer);

	
	// output some text, uses the last used colour
	m_renderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_renderer->end();
}