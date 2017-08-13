#include "Entities\Enemies\Enemy.h"

#include "Behaviours\BSeek.h"
#include "Behaviours\BWander.h"

Enemy::Enemy(aie::Texture * tex, Game_AI_AaronApp * app) : GameObject(tex, app) {
	m_behaviourController = std::shared_ptr<BehaviourController>(new BehaviourController);

	std::shared_ptr<BWander> wander = std::shared_ptr<BWander>(new BWander());
	wander->setParent(this);
	wander->setStrength(200);
	wander->setPriorityWeight(1);
	wander->setProjectionDistance(50);
	wander->setRadius(100);

	m_behaviourController->addBehaviour(wander);
}

Enemy::~Enemy() {
}

void Enemy::update(float deltaTime) {
	GameObject::update(deltaTime);
	if (m_app != nullptr) {
		std::shared_ptr<BSeek> seeker = std::shared_ptr<BSeek>(new BSeek());
		seeker->setParent(this);
		seeker->setPriorityWeight(1.f);
		seeker->setStrength(200);
		seeker->setInnerRadius(30);
		seeker->setOuterRadius(150);
		seeker->onInnerRadiusEnter([&]() {
			m_behaviourController->removeBehaviour(seeker);
		});
	}
}

void Enemy::render(aie::Renderer2D * renderer) {
	GameObject::render(renderer);
}
