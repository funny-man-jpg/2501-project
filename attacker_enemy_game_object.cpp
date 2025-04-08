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
			return new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, textures[tex_attacker_bullet], GetBearing(), player);
		}
		
		return nullptr;
	}
	void AttackerEnemyGameObject::Update(double delta_time, GLuint* textures)
	{
		if (glm::length(target_->GetPosition() - position_) < PURSUIT_CHANGE_DISTANCE && glm::length(target_->GetPosition() - position_) > CHASE_CHANGE_DISTANCE) {
			state_ = intercepting;
		}
		// if patrolling, do the ellipse
		if (state_ == patrolling) {
			if (retarget_->Finished()) {
				// Random point in angle opening
				float r_num = ((float)rand()) / ((float)RAND_MAX);
				float opening = 5.0 * 3.141592 / 180.0; // Add PI from the glm library
				float r_angle = r_num * 2.0 * opening + angle_ - opening;
				float r = 0.25;
				glm::vec3 target(r * cos(r_angle), r * sin(r_angle), 0.0);
				// Steering to target
				glm::vec3 desired = target;
				glm::vec3 steering = desired + velocity_;
				steering /= glm::length(steering);
				steering *= 0.1; // Adjust force magnitude
				velocity_ += steering;
				// Reset timer to only update wander every 1s
				retarget_->Start(1.0);
			}

		}
		// if intercepting, chase the player
		else if (state_ == intercepting && !exploding_ ) {
			double rads = t_ * FULL_CIRCLE * rps_;
			position_ = glm::vec3(2 * cos(rads) + target_->GetPosition().x, 2 * sin(rads) + target_->GetPosition().y, 0.0);
		}
		if (!exploding_) {
			glm::vec3 prev_position = position_;

			// check if need to change state
			glm::vec3 heading;
			if (state_ != intercepting) {
				position_ += velocity_ * (float)delta_time;
			}
			if (state_ == patrolling) {
				heading = position_ - prev_position;
			}
			else {
				heading =  target_->GetPosition() - position_;
			}

			float angle = glm::atan(heading.y, heading.x);
			
			SetRotation(angle);

			t_ += delta_time;
		}

		GameObject::Update(delta_time, textures);
	}
}
