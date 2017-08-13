#pragma once
#include "Behaviour.h"

#include <glm\vec2.hpp>
#include <jm_shapes.h>
#include <vector>

namespace jm {
	struct Rect;
}
namespace aie {
	class Renderer2D;
}

class BCollisionAvoidance : public Behaviour {
public:
	BCollisionAvoidance();
	virtual ~BCollisionAvoidance();

	/** Perform actions upon activating the behaviour*/
	virtual void entryActions();
	/** Perform the behaviour's actions*/
	virtual void doActions(float deltaTime);
	/** Perform actions upon disabling the behaviour*/
	virtual void exitActions();
	/** Allow for debugging render of behaviours*/
	virtual void debugRender(aie::Renderer2D *renderer);

	/** Sets the maximum distance the parent entity can see obstacles*/
	void setVisionDistance(float val);
	/** Gets the maximum distance the parent entity can see obstacles*/
	const float &getVisionDistance();

	/** Sets the cone of 'vision'*/
	void setFOV(float fov);
	/** Gets the cone of 'vision'*/
	const float& getFOV();

	/** Sets the array of objects that are able to be collided with*/
	void setCollidableObjects(std::vector<jm::Rect> colliders);

protected:
	//const jm::Rect& findMostThreateningObstacle();

	std::vector<jm::Rect> m_collidableObjects;
	float m_visionDistance;
	float m_fov;


	glm::vec2 m_ahead, m_aheadLeft, m_aheadRight;

private:
};