#pragma once
#include <INI.h>
typedef INI<> ini_t;

#include <glm\vec2.hpp>

namespace aie {
	class Renderer2D;
};

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	virtual void update(float dt);
	virtual void render(aie::Renderer2D *renderer);

	void applyForce(const glm::vec2& _force);

	void setPos(const glm::vec2& _pos);
	const glm::vec2 &getPos();

	void setVelocity(const glm::vec2& _velocity);
	const glm::vec2 &getVelocity();

	void setFriction(const float& friction);
	const float &getFriction();

	void wrapScreenBounds();
	void constrainToScreenBounds();
	void destroyOnExitScreen();

protected:
	glm::vec2 m_pos, m_velocity, m_acceleration;
	float m_friction;

private:
};