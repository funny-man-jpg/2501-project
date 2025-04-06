#include "runner_enemy_game_object.h"
#include "bomb.h"

namespace game {
	// Constructor
	RunnerEnemyGameObject::RunnerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
	}

	// handle shooting
	Projectile* RunnerEnemyGameObject::Shoot(GLuint* textures) {
		// change to make sure player is behind (taken care of by checking that enemy is running away)
		if (firing_timer_->Finished() && !exploding_ && (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
			firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

			// drop a bomb
			return new Bomb(position_ - radius_ * GetBearing(), geometry_, shader_, textures[tex_bomb], GetBearing(), player);
		}

		return nullptr;
	}
}
