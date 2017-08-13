#pragma once
#include "../GameObject.h"

class BehaviourController;

class Enemy : public GameObject {
public:
	Enemy(aie::Texture *tex = nullptr, Game_AI_AaronApp *app = nullptr);
	virtual ~Enemy();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);

protected:
	std::shared_ptr<BehaviourController> m_bController;

private:
};