#pragma once
#include <INI.h>
typedef INI<> ini_t;

#include <glm\vec2.hpp>
#include <vector>
#include <memory>

class Behaviour;

class JM_Component;
namespace aie {
	class Renderer2D;
	class Texture;
};

class GameObject {
public:
	GameObject(aie::Texture *tex = nullptr);
	virtual ~GameObject();

	virtual void update(float dt);
	virtual void render(aie::Renderer2D *renderer);

	void applyForce(const glm::vec2& _force);

#pragma region Getters and Setters
	void setPos(const glm::vec2& _pos);
	const glm::vec2 &getPos();

	void setVelocity(const glm::vec2& _velocity);
	const glm::vec2 &getVelocity();

	void setFriction(const float& friction);
	const float &getFriction();

	/** Append components into the object*/
	void addComponent(std::shared_ptr<JM_Component> _component);
	/** Attain external access to a component of a defined type*/
	template<typename T>
	T * getComponentOfType();

	void setDraw(bool val);
	bool isDrawn();

	void setBehaviour(std::shared_ptr<Behaviour> behaviour);
	Behaviour *getBehaviour();

#pragma endregion

	void wrapScreenBounds();
	void constrainToScreenBounds(bool bounce);
	void destroyOnExitScreen();

protected:
	glm::vec2 m_pos, m_velocity, m_acceleration;
	const float maxSpeed = 100;
	float m_friction;
	bool m_drawn;

	aie::Texture *m_tex;

	std::vector<std::shared_ptr<JM_Component>> m_components;
	std::shared_ptr<Behaviour> m_behaviour;

	// Used to compound the force when 'Bouncing' off objects
	const float REFLECTION_FORCE = 250;

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