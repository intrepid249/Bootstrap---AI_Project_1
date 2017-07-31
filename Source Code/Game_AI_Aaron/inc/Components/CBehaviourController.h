#pragma once
#include <Components\JM_Component.h>
#include <memory>
#include <vector>

class Behaviour;

class CBehaviourController : public JM_Component {
	CBehaviourController();
	virtual ~CBehaviourController();

	virtual void update(float deltaTime);
	virtual void render(aie::Renderer2D *renderer);

	void addBehaviour(const char *name, std::shared_ptr<Behaviour> behaviour);

	void setBehaviour(const char *name);

private:
	std::vector<std::shared_ptr<Behaviour>> m_registeredBehaviours;
	Behaviour *m_activeBehaviour;
};