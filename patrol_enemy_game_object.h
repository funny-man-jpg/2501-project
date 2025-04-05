#ifndef PATROL_ENEMY_GAME_OBJECT_H_
#define PATROL_ENEMY_GAME_OBJECT_H_

#include "enemy_game_object.h"

namespace game {
	class PatrolEnemyGameObject : public EnemyGameObject {
		public:
			// Constructor
			PatrolEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);

			// handle shooting
			virtual Projectile* Shoot(GLuint* textures);
	};
}

#endif
