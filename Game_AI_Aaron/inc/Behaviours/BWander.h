#pragma once

namespace aie {
	class Renderer2D;
}

class BWander {
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

	void setProjectionDistance(float val);
	float getProjectionDistance();

protected:
	float m_projectionDistance;

private:
};