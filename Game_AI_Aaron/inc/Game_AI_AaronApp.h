#pragma once
#include "Application.h"
#include <memory>

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

protected:
	aie::Renderer2D*	m_renderer;
	aie::Font*			m_font;

	std::unique_ptr<GameObject> m_player = nullptr;
};