#include "Game_AI_AaronApp.h"
#include <Renderer2D.h>
#include <Texture.h>
#include <Font.h>
#include <Input.h>
#include <glm\glm.hpp>

#include <imgui.h>

#include "GlobalConfig.h"
#include "Entities\GameObject.h"
#include "Entities\Player.h"
#include "Graph\Graph2D.h"

Game_AI_AaronApp::Game_AI_AaronApp() {

}

Game_AI_AaronApp::~Game_AI_AaronApp() {

}

bool Game_AI_AaronApp::startup() {
	
	m_renderer = std::shared_ptr<aie::Renderer2D>(new aie::Renderer2D());

	m_player = std::unique_ptr<Player>(new Player());
	m_player->setPos(glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));

	/// Graph
	m_graph = std::unique_ptr<Graph2D>(new Graph2D());
	generateGraph();

	/// Draw graph
	m_graphRenderer = std::unique_ptr<Graph2DRenderer>(new Graph2DRenderer());
	m_graphRenderer->setGraph(m_graph.get());

	return true;
}

void Game_AI_AaronApp::shutdown() {
}

void Game_AI_AaronApp::update(float deltaTime) {
	INI<> *ini = GlobalConfig::getInstance();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(23/255.f, 97/255.f, 203/255.f, 0.5));
	ImGui::Begin("Debugging", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(-1, 220));

	ImGui::Text("Hold LeftCtrl to interact with player's behaviours");
	ImGui::BulletText("Left Mouse Button: Seek toward a point");
	ImGui::BulletText("Middle Mouse Button: Add a node to a path for the player to follow");
	ImGui::BulletText("Right Mouse Button: Add a point for the player to flee from");

	if (ImGui::CollapsingHeader("Graph")) {
		ini->select("DisplayOptions");
		glm::vec2 windowSize(ini->get("WindowWidth", 1), ini->get("WindowHeight", 1));

		static int rows = 2, cols = 2;
		static int spacing = 50;
		static int xOffset = 50, yOffset = 50;

		ImGui::SliderInt("Rows", &rows, 2, (int)((windowSize.y) / spacing));
		ImGui::SliderInt("Cols", &cols, 2, (int)((windowSize.x - xOffset) / spacing));
		
		ImGui::Separator();

		if (ImGui::Button("Update Graph")) {
			m_graph->clear();

			// Write settings to config file
			ini->select("WorldOptions");
			ini->set("GraphRows", rows);
			ini->set("GraphCols", cols);

			generateGraph();
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();

	updateGraph(deltaTime);

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

	m_player->render(m_renderer.get());
	drawGraph();

	// done drawing sprites
	m_renderer->end();
}

void Game_AI_AaronApp::drawGraph() {
	m_graphRenderer->render(m_renderer.get());

	// Get configuration information about the graph
	INI<> *ini = GlobalConfig::getInstance();
	ini->select("WorldOptions");

	int spacing = ini->get("GraphSpacing", 1);

	// Get the mouse coordinates
	aie::Input *input = aie::Input::getInstance();
	int mx, my;
	input->getMouseXY(&mx, &my);
	glm::vec2 mousePos(mx, my);

	// Get the nodes near the mouse position
	std::vector<Graph2D::Node*> nearbyNodes;
	m_graph->getNearbyNodes(mousePos, spacing * 2.f, nearbyNodes);

	for (auto node : nearbyNodes)
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT)) {
			m_renderer->setRenderColour(1, 1, 1, 0.4f);
			m_renderer->drawLine(mousePos.x, mousePos.y, node->data.x, node->data.y);
			m_renderer->setRenderColour(1, 1, 1, 1);
		}
}

void Game_AI_AaronApp::generateGraph() {
	INI<> *ini = GlobalConfig::getInstance();
	ini->select("WorldOptions");

	int numRows = ini->get("GraphRows", 1), numCols = ini->get("GraphCols", 1);
	int spacing = ini->get("GraphSpacing", 1);
	int xOffset = ini->get("GraphXOffset", 1), yOffset = ini->get("GraphYOffset", 1);

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
		m_graph->getNearbyNodes(nodeA->data, spacing * 2.f, nearbyNodes);

		// Loop through nearby nodes
		for (auto iter2 = nearbyNodes.begin(); iter2 != nearbyNodes.end(); iter2++) {
			Graph2D::Node *nodeB = (*iter2);
			if (nodeA == nodeB)
				continue; // Don't connect a node to itself

			int distBetweenNodes = (int)glm::length(nodeB->data - nodeA->data);
			m_graph->addEdge(nodeA, nodeB, true, distBetweenNodes);
		}
	}
}

void Game_AI_AaronApp::updateGraph(float deltaTime) {
	m_graphRenderer->update(deltaTime);

	// Get configuration information about the graph
	INI<> *ini = GlobalConfig::getInstance();
	ini->select("WorldOptions");

	int spacing = ini->get("GraphSpacing", 1);

	// Get the mouse coordinates
	aie::Input *input = aie::Input::getInstance();
	int mx, my;
	input->getMouseXY(&mx, &my);
	glm::vec2 mousePos(mx, my);

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT)) {
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			// Get the nodes near the mouse position
			std::vector<Graph2D::Node*> nearbyNodes;
			m_graph->getNearbyNodes(mousePos, spacing * 2, nearbyNodes);

			int addedNode = m_graph->addNode(mousePos);

			for (auto node : nearbyNodes) {
				m_graph->addEdge(node, m_graph->getNodeByIdx(addedNode), true, (int)glm::length(m_graph->getNodeByIdx(addedNode)->data - node->data));
			}
		}
	}
}
