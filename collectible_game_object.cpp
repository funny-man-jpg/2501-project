#include "collectible_game_object.h"

namespace game {
	// Constructor
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
		type_ = collectible;
	}
	
	void CollectibleGameObject::Hit(GLuint *textures, GameObject *other) {
		if (other->GetType() == player) {
			collideable_ = false;

			// turn on ghost mode
			ghost_ = 1;
		}
	}
}