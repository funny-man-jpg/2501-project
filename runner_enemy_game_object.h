#ifndef RUNNER_ENEMY_GAME_OBJECT_H_
#define RUNNER_ENEMY_GAME_OBJECT_H_

#include "enemy_game_object.h"

namespace game {
	class RunnerEnemyGameObject : public EnemyGameObject {
		public:
			// Constructor
			RunnerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);

			// handle shooting
			virtual Projectile* Shoot(GLuint* textures);
			void Update(double delta_time, GLuint* textures) override;
	};
}

#endif
