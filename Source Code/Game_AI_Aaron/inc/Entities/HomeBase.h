#pragma once
#include "GameObject.h"

namespace aie {
	class Renderer2D;
	class Texture;
}

class HomeBase : public GameObject {
public:
	HomeBase(aie::Texture *tex = nullptr);
	virtual ~HomeBase();

	/** Perform logic to update values each frame*/
	virtual void update(float dt);
	/** Perform logic to display GameObject to the screen*/
	virtual void render(aie::Renderer2D *renderer);

protected:
private:
};