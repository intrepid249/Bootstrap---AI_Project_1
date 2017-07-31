#pragma once
#include "Behaviour.h"
#include <functional>
#include <glm\vec2.hpp>

class Path;

class BFollowPath : public Behaviour {
public:
	BFollowPath();
	virtual ~BFollowPath();

	/** Perform actions upon activating the behaviour*/
	virtual void entryActions();
	/** Perform the behaviour's actions*/
	virtual void doActions(float deltaTime);
	/** Perform actions upon disabling the behaviour*/
	virtual void exitActions();
	/** Allow for debugging render of behaviours*/
	virtual void debugRender(aie::Renderer2D *renderer);

	/** Set the existing path to follow*/
	void setPath(Path *path);
	/** Get the path currently following*/
	Path *getPath();

	/** Set the 'destination' radius for each of the path nodes*/
	void setNodeRadius(float radius);
	/** Get the 'destination radius of each of the path nodes*/
	float getNodeRadius();

	/** Set the 'patrolling' status*/
	void isPatrolling(bool flag);
	/** Get the 'patrolling' status*/
	bool isPatrolling();

	/** Set the increment level for patrolling
	-- Note: Should be 1 for forwards, -1 for backwards*/
	void setPatrolDir(int increment);
	/** Get the increment for patrolling*/
	int getPatrolDir();

	/** Set the callback function for reaching the final node*/
	void onLastNodeReached(std::function<void()> callback);

protected:
	Path *m_path;
	int m_currentPathNodeIndex, m_patrolDir;
	float m_nodeRadius;
	bool m_isPatrolling;
	glm::vec2 m_lastPos;

	std::function<void()> m_onLastNodeReached;
};