#include "runner_enemy_game_object.h"

namespace game {
	// Constructor
	RunnerEnemyGameObject::RunnerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
	}

	// handle shooting
	Projectile* RunnerEnemyGameObject::Shoot(GLuint* textures) {
		return nullptr;
	}
}
