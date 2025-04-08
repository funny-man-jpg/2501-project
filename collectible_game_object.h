#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {
	// new class for collectible game objects
	class CollectibleGameObject : public GameObject {
		public:
			// Constructor
			CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

			// handle collisions
			virtual void Hit(GLuint *textures, GameObject *other) override;
	};
}

#endif