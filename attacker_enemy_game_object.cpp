#include "attacker_enemy_game_object.h"

namespace game {
	// Constructor
	AttackerEnemyGameObject::AttackerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
	}

	// handle shooting
	Projectile* AttackerEnemyGameObject::Shoot(GLuint* textures) {
		// check if close enough to player (we are always chasing so wanna make sure that we arent shooting from 50 miles off the screen)
		// change to make sure player is in front as well (taken care of by checking that enemy is in pursuit/intercept)
		if (firing_timer_->Finished() && glm::length(target_->GetPosition() - position_) <= SHOOTING_RANGE && !exploding_ 
			&& (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
			firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

			// fire a basic projectile
			return new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, textures[projectile], GetBearing(), player);
		}
		
		return nullptr;
	}
}
