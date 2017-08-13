#pragma once
#include "Application.h"
#include <memory>
#include <vector>
#include <map>
#include <Graph\Graph2D.h>
#include <Graph\Graph2DRenderer.h>
#include <jm_shapes.h>

#include "XML\pugixml.hpp"
#include "Tileset.h"

namespace aie {
	class Renderer2D;
	class Font;
}
class GameObject;
class HomeBase;
class Player;
class Enemy;

class Game_AI_AaronApp : public aie::Application {
public:

	Game_AI_AaronApp();
	virtual ~Game_AI_AaronApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	const std::vector<jm::Rect> &getColliders();
	const std::vector<std::shared_ptr<Enemy>> &getEnemies() { return m_enemies; }
	const std::shared_ptr<Player> &getPlayer() { return m_player; }

	void drawGraph();
	void generateGraph();
	bool generateGraphFromFile();
	void generateGraphEdges();
	void updateGraph(float deltaTime);

	void loadTilesetData();

protected:
	std::shared_ptr<aie::Renderer2D>	m_renderer;

	/// Camera information
	bool allowFreeCameraMovement;
	glm::vec2 m_cameraPos;
	glm::vec2 m_cameraSafeBounds;

	/// Map information
	pugi::xml_document m_mapData;
	std::vector<jm::Tileset> m_tilesets;								// Information about each tileset
	std::vector<std::string> m_tilesetImageNames;						// Store names of the tilesets for subscript accessing texture map
	int m_mapWidth, m_mapHeight, m_tileWidth, m_tileHeight;
	std::vector<jm::TileLayer> m_tileGid_Layers;						// Stores the gid information about each tile in an array
	std::map<std::string, std::vector<jm::Tile>> m_backgroundTiles;		// Data about each tile in the map, indexed by layer name
	std::vector<jm::Rect> m_collisionTiles;							// Data about collider objects;

	/// Game objects
	std::shared_ptr<Player> m_player;

	std::vector<std::shared_ptr<Enemy>> m_enemies;
	//std::vector<std::unique_ptr

	std::vector<std::shared_ptr<HomeBase>> m_baseList;
	std::vector<std::shared_ptr<GameObject>> m_worldObjects;

	/// Graph information
	std::unique_ptr<Graph2D> m_graph;
	std::unique_ptr<Graph2DRenderer> m_graphRenderer;

	int m_graphSpacing;
	glm::vec2 m_mousePos;

	/// ImGui
	bool m_showColliders;
};