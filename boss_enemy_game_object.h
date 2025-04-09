#ifndef BOSS_ENEMY_GAME_OBJECT_H_
#define BOSS_ENEMY_GAME_OBJECT_H_

#include "enemy_game_object.h"

#define SHOOTING_RANGE 5.0

namespace game {
	class BossEnemyObject : public EnemyGameObject {
		public:
			// Constructor
			BossEnemyObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);

			// handle shooting
			virtual Projectile* Shoot(GLuint* textures);
			void Update(double delta_time, GLuint* textures) override;
	};
}

#endif
