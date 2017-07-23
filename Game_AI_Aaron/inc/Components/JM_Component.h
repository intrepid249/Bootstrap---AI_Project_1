#pragma once

class GameObject;
namespace aie {
	class Renderer2D;
}

class JM_Component {
public:
	JM_Component() = default;
	virtual ~JM_Component() = default;

	/** Update members and actions each frame*/
	virtual void update(float dt) {};
	/** Allow for displaying objects to the screen*/
	virtual void render(aie::Renderer2D *renderer) {};

	virtual void setParent(GameObject *parent) { m_parent = parent; }

protected:
	GameObject *m_parent;
};