#include "Entities\Player.h"

#include "Behaviours\BKeyboardControlled.h"
#include "Behaviours\BSeek.h"
#include "Behaviours\BFollowPath.h"
#include "Behaviours\BWander.h"

#include "Graph\Path.h"
#include "Pathfinding\Pathfinder.h"

#include "GlobalConfig.h"
#include "ResourceManager.h"

#include <Font.h>
#include <Input.h>
#include <Renderer2D.h>

#include <imgui.h>

#include <iostream>

Player::Player(aie::Texture *tex) : GameObject(tex), m_startNode(nullptr), m_endNode(nullptr) {
	setFriction(1);

	m_keyboardBehaviour = std::shared_ptr<BKeyboardControlled>(new BKeyboardControlled());
	m_keyboardBehaviour->setParent(this);
	m_keyboardBehaviour->setStrength(PLAYER_MOVEMENT_SPEED);

	m_seekBehaviour = std::shared_ptr<BSeek>(new BSeek());
	m_seekBehaviour->setParent(this);
	m_seekBehaviour->setStrength(PLAYER_MOVEMENT_SPEED);
	m_seekBehaviour->setInnerRadius(20);
	m_seekBehaviour->setOuterRadius(200);
	m_seekBehaviour->onInnerRadiusEnter([this]() {
		setBehaviour(m_keyboardBehaviour);
	});

	m_fleeBehaviour = std::shared_ptr<BSeek>(new BSeek());
	m_fleeBehaviour->setParent(this);
	m_fleeBehaviour->setStrength(-PLAYER_MOVEMENT_SPEED);
	m_fleeBehaviour->setOuterRadius(200);
	m_fleeBehaviour->onOuterRadiusExit([this]() {
		setBehaviour(m_keyboardBehaviour);
	});

	m_path = std::unique_ptr<Path>(new Path());
	m_followPathBehaviour = std::shared_ptr<BFollowPath>(new BFollowPath());
	m_followPathBehaviour->setParent(this);
	m_followPathBehaviour->setPath(m_path.get());
	m_followPathBehaviour->setStrength(PLAYER_MOVEMENT_SPEED);
	m_followPathBehaviour->setNodeRadius(20);
	m_followPathBehaviour->onLastNodeReached([this]() {
		if (m_followPathBehaviour->isPatrolling())
			m_followPathBehaviour->setPatrolDir(m_followPathBehaviour->getPatrolDir() * -1);
		else
			setBehaviour(m_keyboardBehaviour);
	});

	m_wanderBehavour = std::shared_ptr<BWander>(new BWander());
	m_wanderBehavour->setParent(this);
	m_wanderBehavour->setStrength(1);
	m_wanderBehavour->setProjectionDistance(50);
	m_wanderBehavour->setRadius(100);

	setBehaviour(m_keyboardBehaviour);
}

Player::~Player() {
}

void Player::update(float deltaTime) {

#pragma region ImGui
	// Live debugging window
	ImGui::Begin("Debugging");

	if (ImGui::CollapsingHeader("AI")) {
		static bool patrolFlag = true;
		if (ImGui::Checkbox("Patrolling", &patrolFlag))
			m_followPathBehaviour->isPatrolling(patrolFlag);

		static bool wanderFlag = false;
		if (ImGui::Checkbox("Wander", &wanderFlag))
			setBehaviour(m_wanderBehavour);
		if (!wanderFlag)
			setBehaviour(m_keyboardBehaviour);


		// Add a tree
		const char* pathfindingAlgorithms[] = { "dijkstra", "astar" };
		static int itemIndex = -1;
		ImGui::Combo("Combo", &itemIndex, pathfindingAlgorithms, IM_ARRAYSIZE(pathfindingAlgorithms));
		m_algorithm = pathfindingAlgorithms[itemIndex];
	}
	
	ImGui::End();
#pragma endregion

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

				if (m_algorithm == "dijkstra") {
					m_pathfinder->findPath(m_startNode, [this](Graph2D::Node *n) {
						return n == m_endNode;
					});
				} else if (m_algorithm == "astar") {

				}

				while (!m_pathfinder->pathFound()) {
					m_pathfinder->updateSearch();
				}

				m_path->clear();
				Path &p = m_pathfinder->getPath();
				auto pathPoints = p.getPath();
				for (auto iter = pathPoints.rbegin(); iter != pathPoints.rend(); iter++) {
					m_path->addPathSegment((*iter));
				}

				setBehaviour(m_followPathBehaviour);
			}
		}

		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
			m_startNode = m_endNode = nullptr;
			m_pathfinder = std::move(std::unique_ptr<Pathfinder>());
		}
	}

	if (getBehaviour() != m_keyboardBehaviour.get() && !input->getPressedKeys().empty() &&
		!(input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL) || input->isKeyDown(aie::INPUT_KEY_LEFT_ALT) 
			|| input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT) || input->isKeyDown(aie::INPUT_KEY_ENTER))) {
		setBehaviour(m_keyboardBehaviour);
	}
}

void Player::render(aie::Renderer2D * renderer) {
	GameObject::render(renderer);

	char buffer[64];
	if (m_behaviour == m_keyboardBehaviour)
		sprintf_s(buffer, "Keyboard");
	else if (m_behaviour == m_seekBehaviour)
		sprintf_s(buffer, "Seek");
	else if (m_behaviour == m_fleeBehaviour)
		sprintf_s(buffer, "Flee");
	else if (m_behaviour == m_followPathBehaviour)
		sprintf_s(buffer, "Follow Path");
	else if (m_behaviour == m_wanderBehavour)
		sprintf_s(buffer, "Wander");
	else
		sprintf_s(buffer, "Behaviour unknown");

	renderer->drawText(ResourceManager::getFonts()["default"].get(), buffer, 10, 10);
}

void Player::setGraph(Graph2D * graph) {
	m_graph = graph;
}

Graph2D * Player::getGraph() {
	return m_graph;
}
