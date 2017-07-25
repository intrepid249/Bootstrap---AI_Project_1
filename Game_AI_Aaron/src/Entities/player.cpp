#include "Entities\Player.h"

#include "Behaviours\BKeyboardControlled.h"
#include "Behaviours\BSeek.h"
#include "Behaviours\BFollowPath.h"

#include "Graph\Path.h"
#include "Pathfinding\Pathfinder.h"

#include "GlobalConfig.h"

#include <Font.h>
#include <Input.h>
#include <Renderer2D.h>

#include <imgui.h>


Player::Player() : GameObject(), m_startNode(nullptr), m_endNode(nullptr) {
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
		if (m_followPathBehaviour->isPatrolling())
			m_followPathBehaviour->setPatrolDir(m_followPathBehaviour->getPatrolDir() * -1);
		else
			setBehaviour(m_keyboardBehaviour);
	});

	setBehaviour(m_keyboardBehaviour);
}

Player::~Player() {
}

void Player::update(float deltaTime) {

	// Add a live debugging window
#ifdef _DEBUG
	ImGui::Begin("Debugging");

	if (ImGui::CollapsingHeader("AI")) {
		static bool flag = false;
		if (ImGui::Checkbox("Patrolling", &flag));
		m_followPathBehaviour->isPatrolling(flag);
	}
	
	ImGui::End();
#endif // _DEBUG

	GameObject::update(deltaTime);

	aie::Input *input = aie::Input::getInstance();

	int mx, my;
	input->getMouseXY(&mx, &my);
	glm::vec2 mousePos(mx, my);

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL)) {
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			m_seekBehaviour->setTarget(mousePos);
			setBehaviour(m_seekBehaviour);
		} else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
			m_fleeBehaviour->setTarget(mousePos);
			setBehaviour(m_fleeBehaviour);
		} else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
			if (getBehaviour() != m_followPathBehaviour.get()) {
				setBehaviour(m_followPathBehaviour);
				m_path->clear();
			}

			m_path->addPathSegment(mousePos);
		}
	}

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) {
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			std::vector<Graph2D::Node*> nearbyNodes;
			m_graph->getNearbyNodes(mousePos, 15, nearbyNodes);

			if (!nearbyNodes.empty()) {
				m_startNode = nearbyNodes[0];
			}
		}

		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
			std::vector<Graph2D::Node*> nearbyNodes;
			m_graph->getNearbyNodes(mousePos, 15, nearbyNodes);

			if (!nearbyNodes.empty()) {
				m_endNode = nearbyNodes[0];
				m_pathfinder = std::move(std::unique_ptr<Pathfinder>(new Pathfinder()));
				m_pathfinder->findPath(m_startNode, [this](Graph2D::Node *n) {
					return n == m_endNode;
				});

				while (m_pathfinder->pathFound()) {
					m_pathfinder->updateSearch();
				}
				Path p = m_pathfinder->getPath();
					
				m_path->clear();
				m_path->addPathSegment(m_startNode->data);
				m_path->addPathSegment(m_endNode->data);

				setBehaviour(m_followPathBehaviour);
			}
		}

		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
			m_startNode = m_endNode = nullptr;
			m_pathfinder = std::move(std::unique_ptr<Pathfinder>());
		}
	}

	if (getBehaviour() != m_keyboardBehaviour.get() && !input->getPressedKeys().empty() &&
		!(input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL) || input->isKeyDown(aie::INPUT_KEY_LEFT_ALT) || input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))) {
		setBehaviour(m_keyboardBehaviour);
	}
}

void Player::render(aie::Renderer2D * renderer) {
	GameObject::render(renderer);

#ifdef _DEBUG
	if (m_startNode != nullptr) {
		renderer->setRenderColour(0.2, 0.8, 0.1, 1);
		renderer->drawCircle(m_startNode->data.x, m_startNode->data.y, 6, 1);
		renderer->setRenderColour(1, 1, 1, 1);
	}

	if (m_endNode != nullptr) {
		renderer->setRenderColour(0.8, 0.2, 0.1, 1);
		renderer->drawCircle(m_endNode->data.x, m_endNode->data.y, 6, 1);
		renderer->setRenderColour(1, 1, 1, 1);
	}
#endif // _DEBUG

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

void Player::setGraph(Graph2D * graph) {
	m_graph = graph;
}

Graph2D * Player::getGraph() {
	return m_graph;
}
