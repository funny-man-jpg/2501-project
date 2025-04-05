#include "patrol_enemy_game_object.h"

namespace game {
	// Constructor
	PatrolEnemyGameObject::PatrolEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
	}

	// handle shooting
	Projectile* PatrolEnemyGameObject::Shoot(GLuint* textures) {
		return nullptr;
	}
}
