#ifndef ATTACKER_ENEMY_GAME_OBJECT_H_
#define ATTACKER_ENEMY_GAME_OBJECT_H_

#include "enemy_game_object.h"

#define SHOOTING_RANGE 5.0

namespace game {
	class AttackerEnemyGameObject : public EnemyGameObject {
		public:
			// Constructor
			AttackerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);

			// handle shooting
			virtual Projectile* Shoot(GLuint* textures);
	};
}

#endif
