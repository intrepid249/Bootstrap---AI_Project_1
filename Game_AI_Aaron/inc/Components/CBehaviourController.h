#pragma once
#include <Components\JM_Component.h>

class CBehaviourController : public JM_Component {
	CBehaviourController();
	virtual ~CBehaviourController();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);
};