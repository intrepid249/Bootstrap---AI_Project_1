#pragma once
#include <glm\vec2.hpp>
#include <vector>
#include "Tileset.h"

class BCollisionAvoidance {
public:
	BCollisionAvoidance();
	virtual ~BCollisionAvoidance();

protected:
	std::vector<jm::Object> m_collidableObjects;

private:
};