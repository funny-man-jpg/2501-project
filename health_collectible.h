#ifndef HEALTH_COLLECTIBLE_H_
#define HEALTH_COLLECTIBLE_H_

#include "collectible_game_object.h"

namespace game {
	class HealthCollectible : public CollectibleGameObject {
	public:
		// Constructor
		HealthCollectible(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
	};
}

#endif