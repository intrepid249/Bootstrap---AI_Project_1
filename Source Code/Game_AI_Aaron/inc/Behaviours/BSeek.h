#pragma once
#include "Behaviour.h"
#include <glm\vec2.hpp>
#include <functional>

class BSeek : public Behaviour {
public:
	BSeek();
	virtual ~BSeek();

	/** Perform actions upon activating the behaviour*/
	virtual void entryActions();
	/** Perform the behaviour's actions*/
	virtual void doActions(float deltaTime);
	/** Perform actions upon disabling the behaviour*/
	virtual void exitActions();
	/** Allow for debugging render of behaviours*/
	virtual void debugRender(aie::Renderer2D *renderer);

	/** Set the target position to seek at*/
	void setTarget(const glm::vec2& target);
	/** Get the target position that's being sought*/
	const glm::vec2& getTarget();

	/** Set the size of the 'destination' area*/
	void setInnerRadius(float radius);
	/** Get the size of the 'destination' area*/
	float getInnerRadius();

	/** Set the size of the 'agro' area*/
	void setOuterRadius(float radius);
	/** Get the size of the 'agro' area*/
	float getOuterRadius();

	/** Set the callback function for entering 'destination' zone*/
	void onInnerRadiusEnter(std::function<void()> callback);
	/** Set the callback function for leaving 'destination' zone*/
	void onInnerRadiusExit(std::function<void()> callback);
	/** Set the callback function for entering 'agro' zone*/
	void onOuterRadiusEnter(std::function<void()> callback);
	/** Set the callback function for leaving 'agro' zone*/
	void onOuterRadiusExit(std::function<void()> callback);

protected:
	glm::vec2 m_targetPos;

	float m_innerRadius, m_outerRadius;
	std::function<void()> m_onInnerRadiusEnter, m_onInnerRadiusExit;
	std::function<void()> m_onOuterRadiusEnter, m_onOuterRadiusExit;

private:
	glm::vec2 m_lastPos;
	
};