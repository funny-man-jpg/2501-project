#include "health_collectible.h"

namespace game {
	// Constructor
	HealthCollectible::HealthCollectible(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) 
		: CollectibleGameObject(position, geom, shader, texture) {

		type_ = health_collectible;
		scale_ = glm::vec2(0.5f);
		radius_ = 0.3f;
	}
}