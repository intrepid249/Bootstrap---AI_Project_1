#include "Entities\Player.h"

#include "Behaviours\BehaviourController.h"
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

Player::Player(aie::Texture *tex) : GameObject(tex) {
	setFriction(1);


	m_keyboardBehaviour = std::shared_ptr<BKeyboardControlled>(new BKeyboardControlled());
	m_keyboardBehaviour->setParent(this);
	m_keyboardBehaviour->setStrength(PLAYER_MOVEMENT_SPEED);

	m_seekBehaviour = std::shared_ptr<BSeek>(new BSeek());
	m_seekBehaviour->setParent(this);
	m_seekBehaviour->setStrength(PLAYER_MOVEMENT_SPEED);
	m_seekBehaviour->setInnerRadius(20);
	m_seekBehaviour->setOuterRadius(200);
	m_seekBehaviour->setPriorityWeight(2.f);
	m_seekBehaviour->onInnerRadiusEnter([this]() {
		m_behaviourController->removeBehaviour(m_seekBehaviour);
	});

	m_fleeBehaviour = std::shared_ptr<BSeek>(new BSeek());
	m_fleeBehaviour->setParent(this);
	m_fleeBehaviour->setStrength(-PLAYER_MOVEMENT_SPEED);
	m_fleeBehaviour->setOuterRadius(200);
	m_fleeBehaviour->onOuterRadiusExit([this]() {
		m_behaviourController->removeBehaviour(m_fleeBehaviour);
	});

	m_path = std::unique_ptr<Path>(new Path());
	m_followPathBehaviour = std::shared_ptr<BFollowPath>(new BFollowPath());
	m_followPathBehaviour->setParent(this);
	m_followPathBehaviour->setPath(m_path.get());
	m_followPathBehaviour->setStrength(PLAYER_MOVEMENT_SPEED);
	m_followPathBehaviour->setNodeRadius(40);
	m_followPathBehaviour->setPriorityWeight(2);
	m_followPathBehaviour->onLastNodeReached([this]() {
		if (m_followPathBehaviour->isPatrolling())
			m_followPathBehaviour->setPatrolDir(m_followPathBehaviour->getPatrolDir() * -1);
		else
			m_behaviourController->removeBehaviour(m_followPathBehaviour);
	});

	m_wanderBehavour = std::shared_ptr<BWander>(new BWander());
	m_wanderBehavour->setParent(this);
	m_wanderBehavour->setStrength(1);
	m_wanderBehavour->setProjectionDistance(50);
	m_wanderBehavour->setRadius(100);
	m_wanderBehavour->setPriorityWeight(1);

	m_behaviourController->addBehaviour(m_wanderBehavour);
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
			m_behaviourController->addBehaviour(m_wanderBehavour);
		if (!wanderFlag)
			m_behaviourController->removeBehaviour(m_wanderBehavour);


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

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL)) {
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			std::shared_ptr<BSeek> seek = std::shared_ptr<BSeek>(new BSeek());
			seek->setParent(this);
			seek->setStrength(PLAYER_MOVEMENT_SPEED);
			seek->setInnerRadius(20);
			seek->setOuterRadius(200);
			seek->setPriorityWeight(2.f);
			seek->onInnerRadiusEnter([this, seek]() {
				m_behaviourController->removeBehaviour(seek);
			});
			seek->setTarget(m_mousePos);
			m_behaviourController->addBehaviour(seek);
		} else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
			m_fleeBehaviour->setTarget(m_mousePos);
			m_behaviourController->addBehaviour(m_fleeBehaviour);
		} else if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
			m_behaviourController->addBehaviour(m_followPathBehaviour);

			m_path->addPathSegment(m_mousePos);
		} else if (input->wasKeyPressed(aie::INPUT_KEY_R))
			m_path->clear();
	}

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) {
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			std::vector<Graph2D::Node*> nearbyNodes;
			m_graph->getNearbyNodes(m_mousePos, 15, nearbyNodes);

			if (!nearbyNodes.empty()) {
				m_startNode = nearbyNodes[0];
			}
		}

		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
			std::vector<Graph2D::Node*> nearbyNodes;
			m_graph->getNearbyNodes(m_mousePos, 15, nearbyNodes);

			if (!nearbyNodes.empty()) {
				m_endNode = nearbyNodes[0];
				m_pathfinder = std::move(std::unique_ptr<Pathfinder>(new Pathfinder()));

				if (m_algorithm == "dijkstra") {
					m_pathfinder->findPath(m_startNode, [this](Graph2D::Node *n) {
						return n == m_endNode;
					});
				} else if (m_algorithm == "astar") {
					m_pathfinder->findPath(m_startNode, m_endNode, [this]() { return glm::length(m_startNode->data - m_endNode->data); });
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

				m_behaviourController->addBehaviour(m_followPathBehaviour);
			}
		}

		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_MIDDLE)) {
			m_startNode = m_endNode = nullptr;
			m_pathfinder = std::move(std::unique_ptr<Pathfinder>());
		}
	}

	//if (getBehaviour() != m_keyboardBehaviour.get() && !input->getPressedKeys().empty() &&
	//	!(input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL) || input->isKeyDown(aie::INPUT_KEY_LEFT_ALT) 
	//		|| input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT) || input->isKeyDown(aie::INPUT_KEY_ENTER))) {
	//	setBehaviour(m_keyboardBehaviour);
	//}
}

void Player::render(aie::Renderer2D * renderer) {
	GameObject::render(renderer);
}

void Player::setMousePos(const glm::vec2 &mousePos) {
	m_mousePos = mousePos;
}
