#pragma once
#include <INI.h>
typedef INI<> ini_t;

#include <glm\vec2.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Graph\Graph2D.h"

class Game_AI_AaronApp;

class Behaviour;
class BehaviourController;

class Path;
class Pathfinder;

class JM_Component;
namespace jm {
	struct Rect;
}
namespace aie {
	class Renderer2D;
	class Texture;
};

class GameObject {
public:
	GameObject(aie::Texture *tex = nullptr, Game_AI_AaronApp *app = nullptr);
	virtual ~GameObject();

	/** Perform logic to update values each frame*/
	virtual void update(float dt);
	/** Perform logic to display GameObject to the screen*/
	virtual void render(aie::Renderer2D *renderer, float depth = 0);

	/** Apply a newtonian force to the velocity of the object*/
	void applyForce(const glm::vec2& _force);

#pragma region Getters and Setters
	/** Sets the position of the object within the world space*/
	void setPos(const glm::vec2& _pos);
	/** Gets the position of the object within the world space*/
	const glm::vec2 &getPos();

	/** Scale the size of the image by specified amount*/
	void scale(const float _scale);
	/** Scale the size of the image by specified amount*/
	void scale(const glm::vec2& _scale);
	/** Get the amount of scaling applied*/
	const glm::vec2 getSize();

	/** Set the current travel velocity of the object*/
	void setVelocity(const glm::vec2& _velocity);
	/** Get the current travel velocity of the object*/
	const glm::vec2 &getVelocity();

	/** Sets the strength of the opposing force to apply to the velocity*/
	void setFriction(const float& friction);
	/** Gets the strength of the opposing force to apply to the velocity*/
	const float &getFriction();

	/** Set the graph that's being used for pathfinding*/
	void setGraph(Graph2D *graph);
	/** Get the graph that's being used for pathfinding*/
	Graph2D *getGraph();

	/** Append components into the object*/
	void addComponent(std::shared_ptr<JM_Component> _component);
	/** Attain external access to a component of a defined type*/
	template<typename T>
	T * getComponentOfType();

	/** Sets the current display status of the object*/
	void setDraw(bool flag);
	/** Gets the current display status of the object*/
	bool isDrawn();

	/** Set the angle of rotation applied to the sprite image and other rotation calculations*/
	void setRotation(float angle);
	/** Get the angle of rotation applied to the sprite image and other rotation calculations*/
	float getRotation();

	/** Get the BehaviourController*/
	const std::shared_ptr<BehaviourController> &getBehaviourController();

#pragma endregion
	/** Perform collision checking and reaction updates*/
	void checkCollisions(const std::vector<jm::Rect> &objList);

	/** When object exits one edge of the screen, move them to the opposite edge*/
	void wrapScreenBounds(glm::vec2 cameraPos = glm::vec2(0));
	/** When the object hits the edge of the window, bounce off and change direction*/
	void constrainToScreenBounds(bool bounce, glm::vec2 cameraPos = glm::vec2(0));
	/** Flag the object for unuse when it exits screen boundaries*/
	void destroyOnExitScreen(glm::vec2 cameraPos = glm::vec2(0));

protected:
	glm::vec2 m_pos, m_size, m_velocity, m_acceleration, m_scaleAmount;
	const float maxSpeed = 100;
	float m_rotation;
	float m_friction;
	bool m_drawn;

	aie::Texture *m_tex;

	Graph2D *m_graph;
	std::unique_ptr<Path> m_path;
	Graph2D::Node *m_startNode, *m_endNode;
	std::unique_ptr<Pathfinder> m_pathfinder;

	std::vector<std::shared_ptr<JM_Component>> m_components;
	std::shared_ptr<BehaviourController> m_behaviourController;

	// Used to compound the force when 'Bouncing' off objects
	const float REFLECTION_FORCE = 250;

	Game_AI_AaronApp *m_app;

private:
};

template<typename T>
inline T * GameObject::getComponentOfType() {
	for (auto iter = m_components.begin(); iter != m_components.end(); iter++) {
		if (dynamic_cast<T*>(*iter) != nullptr) {
			return dynamic_cast<T*>(*iter);
		}
	}
	return nullptr;
}