#pragma once
#include "../GameObject.h"

class Enemy : public GameObject {
public:
	Enemy(aie::Texture *tex);
	virtual ~Enemy();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);

protected:
private:
};