#pragma once
#include "GameObject.h"

#include <map>

class BKeyboardControlled;
class BSeek;
class BFollowPath;
class Path;

namespace aie {
	class Font;
}


class Player : public GameObject {
public:
	Player();
	virtual ~Player();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);

protected:
	std::shared_ptr<BKeyboardControlled> m_keyboardBehaviour;
	std::shared_ptr<BSeek> m_seekBehaviour, m_fleeBehaviour;
	std::shared_ptr<BFollowPath> m_followPathBehaviour;

	std::unique_ptr<Path> m_path;

	std::unique_ptr<aie::Font> m_font;

private:
};