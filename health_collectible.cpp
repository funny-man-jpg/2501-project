#include "health_collectible.h"

namespace game {
	// Constructor
	HealthCollectible::HealthCollectible(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) 
		: CollectibleGameObject(position, geom, shader, texture) {
	}
}