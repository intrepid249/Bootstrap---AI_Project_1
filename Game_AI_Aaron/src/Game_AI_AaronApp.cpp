#include "Game_AI_AaronApp.h"
#include <Renderer2D.h>
#include <Texture.h>
#include <Font.h>
#include <Input.h>
#include <glm\glm.hpp>

#include <imgui.h>
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "GlobalConfig.h"
#include "Entities\GameObject.h"
#include "Entities\Player.h"
#include "Graph\Graph2D.h"

#include "ResourceManager.h"

Game_AI_AaronApp::Game_AI_AaronApp() {

}

Game_AI_AaronApp::~Game_AI_AaronApp() {

}

bool Game_AI_AaronApp::startup() {
	
	m_renderer = std::shared_ptr<aie::Renderer2D>(new aie::Renderer2D());

	/// Graph
	m_graph = std::unique_ptr<Graph2D>(new Graph2D());
	generateGraph();

	/// Draw graph
	m_graphRenderer = std::unique_ptr<Graph2DRenderer>(new Graph2DRenderer());
	m_graphRenderer->setGraph(m_graph.get());

	/// Resources
	ResourceManager::create();
	ResourceManager::getTextures()["player"] = std::shared_ptr<aie::Texture>(new aie::Texture("./textures/ship.png"));


	/// Game Objects
	m_player = std::unique_ptr<Player>(new Player(ResourceManager::getTextures()["player"].get()));
	m_player->setPos(glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));
	m_player->setGraph(m_graph.get());


	return true;
}

void Game_AI_AaronApp::shutdown() {
	ResourceManager::destroy();
}

void Game_AI_AaronApp::update(float deltaTime) {
	INI<> *ini = GlobalConfig::getInstance();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(23/255.f, 97/255.f, 203/255.f, 0.5));
	ImGui::Begin("Debugging", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(-1, 250));

	ImGui::Text("Hold LeftCtrl to interact with player's behaviours");
	ImGui::BulletText("Left Mouse Button: Seek toward a point");
	ImGui::BulletText("Middle Mouse Button: Add a node to a path for the player to follow");
	ImGui::BulletText("Right Mouse Button: Add a point for the player to flee from");
	ImGui::Text("Hold LeftAlt to edit the graph");
	ImGui::BulletText("Left Mouse Button: Add a node to the graph");
	ImGui::Text("Hold LeftShift to use the path tools on the graph");
	ImGui::BulletText("Left Mouse Button: Set the start node of the path");
	ImGui::BulletText("Middle Mouse Button: Clear the path information");
	ImGui::BulletText("Right Mouse Button: Set the end node of the path");

	//const char* items[] = { "AAAA", "BBBB" };
	//static int itemIndex = -1;
	//ImGui::Combo("Combo", &itemIndex, items, IM_ARRAYSIZE(items));
	//if (itemIndex == 1) {
	//	quit();
	//}

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

	// Get the mouse position
	int mx, my;
	input->getMouseXY(&mx, &my);
	m_mousePos = glm::vec2(mx, my);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Game_AI_AaronApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_renderer->begin();

	drawGraph();
	m_player->render(m_renderer.get());

	// done drawing sprites
	m_renderer->end();
}

void Game_AI_AaronApp::drawGraph() {
	m_graphRenderer->render(m_renderer.get());

	aie::Input *input = aie::Input::getInstance();

	// Get the nodes near the mouse position
	std::vector<Graph2D::Node*> nearbyNodes;
	m_graph->getNearbyNodes(m_mousePos, m_graphSpacing * 2.f, nearbyNodes);

	for (auto node : nearbyNodes)
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT)) {
			m_renderer->setRenderColour(1, 1, 1, 0.4f);
			m_renderer->drawLine(m_mousePos.x, m_mousePos.y, node->data.x, node->data.y);
			m_renderer->setRenderColour(1, 1, 1, 1);
		}
}

void Game_AI_AaronApp::generateGraph() {
	INI<> *ini = GlobalConfig::getInstance();
	ini->select("WorldOptions");

	int numRows = ini->get("GraphRows", int()), numCols = ini->get("GraphCols", int());
	m_graphSpacing = ini->get("GraphSpacing", int());
	int xOffset = ini->get("GraphXOffset", int()), yOffset = ini->get("GraphYOffset", int());

	// Populate the graph with data
	for (int y = 0; y < numRows; y++) {
		for (int x = 0; x < numCols; x++) {
			m_graph->addNode(glm::vec2(x * m_graphSpacing + xOffset, y * m_graphSpacing + yOffset));
		}
	}

	// Connect all nearby nodes
	auto &nodes = m_graph->getNodes();
	for (auto iter1 = nodes.begin(); iter1 != nodes.end(); iter1++) {
		Graph2D::Node *nodeA = (*iter1).get();
		std::vector<Graph2D::Node*> nearbyNodes;
		m_graph->getNearbyNodes(nodeA->data, m_graphSpacing * 2.f, nearbyNodes);

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

	aie::Input *input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT)) {
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			// Get the nodes near the mouse position
			std::vector<Graph2D::Node*> nearbyNodes;
			m_graph->getNearbyNodes(m_mousePos, m_graphSpacing * 2.f, nearbyNodes);

			int addedNode = m_graph->addNode(m_mousePos);

			for (auto node : nearbyNodes) {
				m_graph->addEdge(node, m_graph->getNodeByIdx(addedNode), true, (int)glm::length(m_graph->getNodeByIdx(addedNode)->data - node->data));
			}
		}
	}
}
