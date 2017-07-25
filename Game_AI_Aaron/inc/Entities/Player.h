#pragma once
#include "GameObject.h"
#include "Graph\Graph2D.h"

#include <map>

class BKeyboardControlled;
class BSeek;
class BFollowPath;

class Path;
class Pathfinder;

namespace aie {
	class Font;
}


class Player : public GameObject {
public:
	Player();
	virtual ~Player();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);

	void setGraph(Graph2D *graph);
	Graph2D *getGraph();

protected:
	std::shared_ptr<BKeyboardControlled> m_keyboardBehaviour;
	std::shared_ptr<BSeek> m_seekBehaviour, m_fleeBehaviour;
	std::shared_ptr<BFollowPath> m_followPathBehaviour;

	std::unique_ptr<Path> m_path;
	Graph2D *m_graph;
	Graph2D::Node *m_startNode, *m_endNode;
	std::unique_ptr<Pathfinder> m_pathfinder;

	std::unique_ptr<aie::Font> m_font;

private:
};