#include "collectible_game_object.h"

namespace game {
	// Constructor
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
		type_ = invincibility_collectible;
	}
	
	void CollectibleGameObject::Hit(GLuint *textures, GameObject *other) {
		if (other->GetType() == player) {
			collideable_ = false;

			// set to be deleted
			delete_ = true;
		}
	}
}