#pragma once
#include "Behaviour.h"
#include <memory>

namespace aie {
	class Renderer2D;
}
class BSeek;

class BWander : public Behaviour {
public:
	BWander();
	virtual ~BWander();

	/** Perform actions upon activating the behaviour*/
	virtual void entryActions();
	/** Perform the behaviour's actions*/
	virtual void doActions(float deltaTime);
	/** Perform actions upon disabling the behaviour*/
	virtual void exitActions();
	/** Allow for debugging render of behaviours*/
	virtual void debugRender(aie::Renderer2D *renderer);

	/** Sets the distance to project in front of the Entity, which
	will affect the influence on the speed the Entity travels at*/
	void setProjectionDistance(const float val);
	/** Gets the distance projected in front of the Entity, which
	affects the influence on the speed the Entity travels at*/
	float getProjectionDistance() const;

	/** Set the radius that will determine the amount of variation
	to the current heading*/
	void setRadius(const float val);
	/** Get the radius that determines the amount of variation to
	the current heading*/
	float getRadius() const;

protected:
	float m_projectionDistance;
	float m_radius;
	float m_newDirDelay = 0.3f;
	const int ANGLE_CHANGE = 90;

	std::shared_ptr<BSeek> m_seeker;

private:
};