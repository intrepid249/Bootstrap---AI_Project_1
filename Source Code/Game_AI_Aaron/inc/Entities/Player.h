#pragma once
#include "GameObject.h"

#include <map>

class Game_AI_AaronApp;

class BKeyboardControlled;
class BSeek;
class BFollowPath;
class BWander;
class BCollisionAvoidance;

class Path;
class Pathfinder;

namespace aie {
	class Font;
	class Texture;
}


class Player : public GameObject {
public:
	Player(aie::Texture *tex = nullptr, Game_AI_AaronApp *app = nullptr);
	virtual ~Player();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);

	void setMousePos(const glm::vec2 &mousePos);

protected:
	std::shared_ptr<BKeyboardControlled> m_keyboardBehaviour;
	std::shared_ptr<BSeek> m_seekBehaviour, m_fleeBehaviour;
	std::shared_ptr<BFollowPath> m_followPathBehaviour;
	std::shared_ptr<BWander> m_wanderBehavour;
	std::shared_ptr<BCollisionAvoidance> m_collisionAvoidance;

	std::unique_ptr<aie::Font> m_font;

	glm::vec2 m_mousePos;

private:
	const char *m_algorithm;

};