#include "Entities\Player.h"
#include "Behaviours\BKeyboardControlled.h"
#include "Behaviours\BSeek.h"

#include "Graph\Path.h"
#include "Behaviours\BFollowPath.h"

#include <Font.h>
#include <Input.h>
#include <Renderer2D.h>

Player::Player() : GameObject() {
	m_font = std::unique_ptr<aie::Font>(new aie::Font("./font/consolas.ttf", 18));

	setFriction(1);

	m_keyboardBehaviour = std::shared_ptr<BKeyboardControlled>(new BKeyboardControlled());
	m_keyboardBehaviour->setParent(this);
	m_keyboardBehaviour->setStrength(100);

	m_seekBehaviour = std::shared_ptr<BSeek>(new BSeek());
	m_seekBehaviour->setParent(this);
	m_seekBehaviour->setStrength(100);
	m_seekBehaviour->setInnerRadius(20);
	m_seekBehaviour->onInnerRadiusEnter([this]() {
		setBehaviour(m_keyboardBehaviour);
	});

	m_fleeBehaviour = std::shared_ptr<BSeek>(new BSeek());
	m_fleeBehaviour->setParent(this);
	m_fleeBehaviour->setStrength(-100);
	m_fleeBehaviour->setOuterRadius(100);
	m_fleeBehaviour->onOuterRadiusExit([this]() {
		setBehaviour(m_keyboardBehaviour);
	});

	m_path = std::unique_ptr<Path>(new Path());
	m_followPathBehaviour = std::shared_ptr<BFollowPath>(new BFollowPath());
	m_followPathBehaviour->setParent(this);
	m_followPathBehaviour->setPath(m_path.get());
	m_followPathBehaviour->setStrength(80);
	m_followPathBehaviour->setNodeRadius(20);
	m_followPathBehaviour->onLastNodeReached([this]() {
		m_followPathBehaviour->setPatrolDir(m_followPathBehaviour->getPatrolDir() * -1);
	});

	setBehaviour(m_keyboardBehaviour);
}

Player::~Player() {
	setBehaviour(nullptr);
}

void Player::update(float deltaTime) {
	GameObject::update(deltaTime);

	aie::Input *input = aie::Input::getInstance();

	int mx, my;
	input->getMouseXY(&mx, &my);
	glm::vec2 mousePos(mx, my);

	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		m_seekBehaviour->setTarget(mousePos);
		setBehaviour(m_seekBehaviour);
	} 
	else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
		m_fleeBehaviour->setTarget(mousePos);
		setBehaviour(m_fleeBehaviour);
	} 
	else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
		if (getBehaviour() != m_followPathBehaviour.get()) {
			setBehaviour(m_followPathBehaviour);
			m_path->clear();
		}

		m_path->addPathSegment(mousePos);
	}

	if (getBehaviour() != m_keyboardBehaviour.get() && !input->getPressedKeys().empty()) {
		setBehaviour(m_keyboardBehaviour);
	}
}

void Player::render(aie::Renderer2D * renderer) {
	GameObject::render(renderer);

	char buffer[64];
	if (m_behaviour == m_keyboardBehaviour)
		sprintf_s(buffer, "Keyboard");
	if (m_behaviour == m_seekBehaviour)
		sprintf_s(buffer, "Seek");
	if (m_behaviour == m_fleeBehaviour)
		sprintf_s(buffer, "Flee");
	if (m_behaviour == m_followPathBehaviour)
		sprintf_s(buffer, "Follow Path");

	renderer->drawText(m_font.get(), buffer, 10, 10);
}
