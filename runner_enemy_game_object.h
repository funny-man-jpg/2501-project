#ifndef RUNNER_ENEMY_GAME_OBJECT_H_
#define RUNNER_ENEMY_GAME_OBJECT_H_

#include "enemy_game_object.h"

// emp ammo drop rate
#define EMP_AMMO_DROP_CHANCE 2

namespace game {
	class RunnerEnemyGameObject : public EnemyGameObject {
		public:
			// Constructor
			RunnerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);

			// handle shooting
			virtual std::vector<Projectile*>* Shoot(GLuint* textures);

			void Render(glm::mat4 view_matrix, double current_time) override;

			void Render(glm::mat4 view_matrix, double current_time, glm::mat4 local);

			void Update(double delta_time, GLuint* textures) override;

			// drop collectibles
			virtual CollectibleGameObject* DropCollectible(GLuint* textures);
	};
}

#endif
