#include "Game_AI_AaronApp.h"
#include <Renderer2D.h>
#include <Texture.h>
#include <Font.h>
#include <Input.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

#include <fstream>
#include <ios>
#include <Windows.h>

#include <imgui.h>
#include <string>
#include <algorithm>

#include <iostream>

#include <jm_utilities.h>

#include "GlobalConfig.h"
#include "Entities\GameObject.h"
#include "Entities\Player.h"
#include "Entities\HomeBase.h"

#include "Graph\Graph2D.h"

#include "ResourceManager.h"

Game_AI_AaronApp::Game_AI_AaronApp() {

}

Game_AI_AaronApp::~Game_AI_AaronApp() {

}

bool Game_AI_AaronApp::startup() {

	m_renderer = std::shared_ptr<aie::Renderer2D>(new aie::Renderer2D());

	/// Configuration
	INI<> *ini = GlobalConfig::getInstance();

	/// Map Information
	pugi::xml_parse_result result = m_mapData.load_file("./maps/map1.tmx");
	if (result) {
		// Store the map attributes as member variables
		pugi::xml_node map = m_mapData.child("map");
		m_mapWidth = map.attribute("width").as_int();
		m_mapHeight = map.attribute("height").as_int();
		m_tileWidth = map.attribute("tilewidth").as_int();
		m_tileHeight = map.attribute("tileheight").as_int();

		// Iterate through each of the tilesets and store their information for later use
		int tilesetCount = 0;
		for (auto iter = map.children("tileset").begin(); iter != map.children("tileset").end(); iter++) {
			pugi::xml_document tilesetData;
			std::string filepath = "./maps/";
			tilesetData.load_file((filepath + (*iter).attribute("source").value()).c_str());
			pugi::xml_node tilesetDataNode = tilesetData.child("tileset");
			size_t imageWidth = tilesetDataNode.child("image").attribute("width").as_int();
			size_t imageHeight = tilesetDataNode.child("image").attribute("height").as_int();
			size_t firstGid = (*iter).attribute("firstgid").as_int();
			std::string name = tilesetDataNode.attribute("name").as_string();
			size_t tileWidth = tilesetDataNode.attribute("tilewidth").as_int();
			size_t tileHeight = tilesetDataNode.attribute("tileheight").as_int();
			size_t tileCount = tilesetDataNode.attribute("tilecount").as_int();
			size_t columns = tilesetDataNode.attribute("columns").as_int();

			// Load the image file for each tilset
			std::string imagePath = tilesetDataNode.child("image").attribute("source").as_string();
			//std::cout << "Image source " << imagePath << "\n";
			m_tilesetImageNames.push_back(name);
			ResourceManager::getTextures()[m_tilesetImageNames[tilesetCount].c_str()] = std::shared_ptr<aie::Texture>(
				new aie::Texture(imagePath.replace(imagePath.begin(), imagePath.begin() + 1, "").c_str()));

			jm::Tileset ts = jm::Tileset(firstGid, name, tileWidth, tileHeight, tileCount, columns, imageWidth, imageHeight);
			m_tilesets.push_back(std::move(ts));
			tilesetCount++;
		}

		auto objectList = map.child("objectgroup");
		for (auto iter = objectList.children("object").begin(); iter != objectList.children("object").end(); iter++) {
			float cx = (*iter).attribute("x").as_float();
			float cy = (*iter).attribute("y").as_float();
			float width = (*iter).attribute("width").as_float();
			float height = (*iter).attribute("height").as_float();
			jm::Object collider(cx, cy, width, height);
			m_collisionTiles.push_back(collider);
		}


		// Iterate through each tile layer to store gid information
		for (auto iter = map.children("layer").begin(); iter != map.children("layer").end(); iter++) {
			// GID data is stored in CSV format
			std::string rawText = (*iter).child("data").text().as_string();
			std::vector<std::string> gidData;
			jm::stringSplit(rawText, ',', gidData);

			// Store the tile gid data
			jm::TileLayer tl((*iter).attribute("name").value(), (*iter).attribute("width").as_int(), (*iter).attribute("height").as_int());
			tl.layerData = gidData;

			m_tileGid_Layers.push_back(tl);
		}

		// Iterate through each tile layer to store tile information
		int currentTilesetIndex = 0;
		size_t layerIndex = 0;
		for (auto iter = map.children("layer").begin(); iter != map.children("layer").end(); iter++) {
			// Iterate through the gid list and store tile information
			auto currentLayer = m_tileGid_Layers[layerIndex];
			std::string layerName = (*iter).attribute("name").as_string();

			for (size_t tileIndex = 0; tileIndex < currentLayer.layerData.size(); tileIndex++) {
				std::cout << currentLayer.layerData[tileIndex] << ", ";
				int tempGid = std::stoi(currentLayer.layerData[tileIndex]); // For some reason all the layer data for "top" layer is 0

				// Adjust the gid for different tilesets and alter the tilesetIndex according to the tile data requested
				if (tempGid > m_tilesets[currentTilesetIndex].tileCount) {
					tempGid -= (currentTilesetIndex == 0) ? m_tilesets[currentTilesetIndex].tileCount : m_tilesets[currentTilesetIndex - 1].tileCount;
					if (currentTilesetIndex + 1 < m_tilesets.size())
						currentTilesetIndex++;
				}
				else if (tempGid != 0 && tempGid < m_tilesets[currentTilesetIndex].firstGID)
					currentTilesetIndex--;

				jm::Tile t;
				t.gid = tempGid - 1;
				if (tempGid != 0)
					t.image = ResourceManager::getTextures()[m_tilesetImageNames[currentTilesetIndex].c_str()].get();
				else
					t.image = nullptr;

				t.width = (float)m_tilesets[currentTilesetIndex].tileWidth;
				t.height = (float)m_tilesets[currentTilesetIndex].tileHeight;
				// Calculate uv coordinates needed for accessing the individual tile image
				t.uv_y = (t.gid == 0) ? 0 : ((t.gid % m_tilesets[currentTilesetIndex].columns) == 0) ? (t.gid / m_tilesets[currentTilesetIndex].columns)
					: (int)ceil(t.gid / (double)m_tilesets[currentTilesetIndex].columns) - 1;
				t.uv_x = t.gid - (m_tilesets[currentTilesetIndex].columns * t.uv_y);

				// Set the rendering depth according to the tileset layer
				std::transform(currentLayer.name.begin(), currentLayer.name.end(), currentLayer.name.begin(), ::tolower);
				if (currentLayer.name == "background")
					t.layerDepth = 20;
				else if (currentLayer.name == "foreground")
					t.layerDepth = -1;
				else
					t.layerDepth = -5;

				m_backgroundTiles[layerName].push_back(t);
			}
			layerIndex++;
		}
	} else {
#if 0
		// TODO: Memory leaks from this part of the code??
		wchar_t *errorText = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, result.description(), -1, errorText, 4096);
		MessageBox(NULL, errorText, L"Error reading map information", MB_OK | MB_ICONERROR);
		delete[] errorText;
#endif

		std::cout << "Problem reading map information:\n" << result.description() << "\n";
	}

	/// Graph
	// Read configuration settings
	ini->select("WorldOptions");
	m_graphSpacing = ini->get("GraphSpacing", int());
	// Generate the graph
	m_graph = std::unique_ptr<Graph2D>(new Graph2D());
	if (!generateGraphFromFile())
		generateGraph();

	/// Draw graph
	m_graphRenderer = std::unique_ptr<Graph2DRenderer>(new Graph2DRenderer());
	m_graphRenderer->setGraph(m_graph.get());

	/// Resources
	ResourceManager::create();
	// Textures
	ResourceManager::getTextures()["appIcon"] = std::shared_ptr<aie::Texture>(new aie::Texture("./textures/Endless/appIcon.png"));
	ResourceManager::getTextures()["player"] = std::shared_ptr<aie::Texture>(new aie::Texture("./textures/Endless/player_handgun.png"));
	ResourceManager::getTextures()["homeBase"] = std::shared_ptr<aie::Texture>(new aie::Texture("./textures/Endless/homeBase.png"));
	// Fonts
	ResourceManager::getFonts()["default"] = std::shared_ptr<aie::Font>(new aie::Font("./font/consolas.ttf", 32));

	/// Game Objects
	// Base
	std::shared_ptr<HomeBase> base = std::shared_ptr<HomeBase>(new HomeBase(ResourceManager::getTextures()["homeBase"].get()));
	base->setPos(glm::vec2(300, 300));
	base->scale(1.3f);
	m_baseList.push_back(std::move(base));
	// Player
	m_player = std::unique_ptr<Player>(new Player(ResourceManager::getTextures()["player"].get()));
	m_player->setPos(glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));
	m_player->scale(0.2f);
	m_player->setGraph(m_graph.get());


	// Change the window icon
	GLFWimage appIcon[1];
	aie::Texture *appImg = ResourceManager::getTextures()["appIcon"].get();
	appIcon[0].width = appImg->getWidth();
	appIcon[0].height = appImg->getHeight();
	appIcon[0].pixels = const_cast<unsigned char*>(appImg->getPixels());
	glfwSetWindowIcon(getWindowPtr(), 1, appIcon);


	// Set to false for standard gameplay
	allowFreeCameraMovement = true;

	return true;
}

void Game_AI_AaronApp::shutdown() {
	ResourceManager::destroy();
}

void Game_AI_AaronApp::update(float deltaTime) {

	INI<> *ini = GlobalConfig::getInstance();

#pragma region ImGui
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(23 / 255.f, 97 / 255.f, 203 / 255.f, 0.75));
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
	ImGui::Text("Note: Please select the pathfinding algorithm to use under the AI\nmenu before generating a path");
	ImGui::BulletText("Left Mouse Button: Set the start node of the path");
	ImGui::BulletText("Middle Mouse Button: Clear the path information");
	ImGui::BulletText("Right Mouse Button: Set the end node of the path");

	if (ImGui::CollapsingHeader("Graph")) {
		ini->select("DisplayOptions");
		glm::vec2 windowSize(ini->get("WindowWidth", int()), ini->get("WindowHeight", int()));

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
			ini->save();

			generateGraph();
		}
		ImGui::SameLine(125);

		if (ImGui::Button("Save Graph")) {
			// Load file for writing

			std::fstream graphFile("config/graph.gdat", std::ios::out | std::ios::binary);

			for (auto node : m_graph->getNodes()) {
				graphFile.write((char*)&node->data, sizeof(node->data));
			}

			graphFile.close();
		}
		ImGui::SameLine(230);

		if (ImGui::Button("Load Graph Data")) {
			if (!generateGraphFromFile())
				MessageBox(NULL, L"Error loading graph information from file! Either the file does not exist, or contains no data", L"Bad graph data!", MB_OK | MB_ICONWARNING);
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();
#pragma endregion

	updateGraph(deltaTime);

	m_player->update(deltaTime);
	//m_player->constrainToScreenBounds(true);
	//m_player->wrapScreenBounds();

	for (auto base : m_baseList)
		base->update(deltaTime);

	// input example
	aie::Input* input = aie::Input::getInstance();

	// Get the mouse position
	int mx, my;
	input->getMouseXY(&mx, &my);
	m_mousePos = glm::vec2(mx, my);

	// Move camera around freely for testing
	if (allowFreeCameraMovement) {
		static const float cameraSpeed = 300.f;
		if (input->isKeyDown(aie::INPUT_KEY_DOWN))
			m_cameraPos.y -= cameraSpeed * deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_UP))
			m_cameraPos.y += cameraSpeed * deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_LEFT))
			m_cameraPos.x -= cameraSpeed * deltaTime;
		if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
			m_cameraPos.x += cameraSpeed * deltaTime;

		// Correct mouse position for new camera location
		m_mousePos += m_cameraPos;
	}

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

	for (auto base : m_baseList)
		base->render(m_renderer.get());

	//aie::Texture *bgImage = ResourceManager::getTextures()[m_tilesetImageNames[0].c_str()].get();
	//m_renderer->drawSprite(bgImage, 100, 100, 0, 0, 0, -1);
#pragma region Tileset
	pugi::xml_node map = m_mapData.child("map");
	for (auto layerIter = map.children("layer").begin(); layerIter != map.children("layer").end(); layerIter++) {
		std::string layerName = (*layerIter).attribute("name").as_string();
		float x = 0, y = 0;
		for (auto iter = m_backgroundTiles[layerName].begin(); iter != m_backgroundTiles[layerName].end(); iter++) {
			jm::Tile t = (*iter);
			if (t.image != nullptr) {
				float ux = (float)(t.uv_x) * (t.width / (float)t.image->getWidth());
				float uy = (float)(t.uv_y) * (t.height / (float)t.image->getHeight());
				// Modify the UV region of the sprite to display the selected tile within the image
				m_renderer->setUVRect(ux, uy, t.width / m_tilesets[0].imageWidth, t.height / m_tilesets[0].imageHeight);

				m_renderer->drawSprite(t.image, x * t.width, (m_mapHeight * m_tileHeight) - (y * t.height), t.width, t.height, 0, t.layerDepth);

				m_renderer->setUVRect(0, 0, 1, 1);
			}

			if (x < m_tileGid_Layers[0].width - 1) {
				x++;
				continue;
			} else
				x = 0;

			y++;
		}
	}

	for (auto iter = m_collisionTiles.begin(); iter != m_collisionTiles.end(); iter++) {
		m_renderer->setRenderColour(1, 0, 0, 0.3);
		jm::Object obj = (*iter);
		m_renderer->drawBox(obj.x + obj.width / 2, (m_mapHeight * m_tileHeight) - (obj.y + obj.height / 2), obj.width, obj.height);
		m_renderer->setRenderColour(1, 1, 1, 1);
	}
#pragma endregion

	m_renderer->setCameraPos(m_cameraPos.x, m_cameraPos.y);

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
	int xOffset = ini->get("GraphXOffset", int()), yOffset = ini->get("GraphYOffset", int());

	// Populate the graph with data
	for (int y = 0; y < numRows; y++) {
		for (int x = 0; x < numCols; x++) {
			m_graph->addNode(glm::vec2(x * m_graphSpacing + xOffset, y * m_graphSpacing + yOffset));
		}
	}

	generateGraphEdges();
}

bool Game_AI_AaronApp::generateGraphFromFile() {
	// Load file for reading
	std::fstream graphFile("config\\graph.gdat", std::ios::in | std::ios::binary);

	// Check for reading errors
	if ((graphFile.rdstate() & std::fstream::failbit) != 0) {
		return false;
	}

	m_graph->clear();

	while (!graphFile.eof() && graphFile.peek() != EOF) {
		Graph2D::Node tmpNode;
		graphFile.read((char *)&tmpNode.data, sizeof(tmpNode.data));
		m_graph->addNode(tmpNode.data);
	}
	generateGraphEdges();

	graphFile.close();
	return true;
}

void Game_AI_AaronApp::generateGraphEdges() {
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

void Game_AI_AaronApp::generateTilesetData() {

}
