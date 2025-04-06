#include "patrol_enemy_game_object.h"
#include "wave_projectile.h"

namespace game {
	// Constructor
	PatrolEnemyGameObject::PatrolEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
	}

	// handle shooting
	Projectile* PatrolEnemyGameObject::Shoot(GLuint* textures) {
		// change to make sure player is in front (taken care of by checking that enemy is in pursuit/intercept)
		if (firing_timer_->Finished() && !exploding_ && (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
			firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

			// fire a wave projectile
			return new WaveProjectile(position_ + radius_ * GetBearing(), geometry_, shader_, textures[tex_wave_bullet], GetBearing(), player);
		}

		return nullptr;
	}
}
