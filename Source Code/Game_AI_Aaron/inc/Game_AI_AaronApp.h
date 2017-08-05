#pragma once
#include "Application.h"
#include <memory>
#include <vector>
#include <Graph\Graph2D.h>
#include <Graph\Graph2DRenderer.h>

#include "XML\pugixml.hpp"
#include "Tileset.h"

#include "Entities\Player.h"


namespace aie {
	class Renderer2D;
	class Font;
}
class GameObject;
class HomeBase;

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

	void generateTilesetData();

protected:
	std::shared_ptr<aie::Renderer2D>	m_renderer;

	/// Map information
	pugi::xml_document m_mapData;
	std::vector<jm::Tileset> m_tilesets;								// Information about each tileset
	std::vector<std::string> m_tilesetImageNames;						// Store names of the tilesets for subscript accessing texture map
	unsigned int m_mapWidth, m_mapHeight, m_tileWidth, m_tileHeight;
	std::vector<jm::TileLayer> m_tileGid_Layers;						// Stores the gid information about each tile in an array
	std::vector<jm::Tile> m_backgroundTiles;							// Data about each tile in the map

	/// Game objects
	std::unique_ptr<Player> m_player;

	std::vector<std::shared_ptr<HomeBase>> m_baseList;
	std::vector<std::shared_ptr<GameObject>> m_worldObjects;

	/// Graph information
	std::unique_ptr<Graph2D> m_graph;
	std::unique_ptr<Graph2DRenderer> m_graphRenderer;

	int m_graphSpacing;
	glm::vec2 m_mousePos;
};