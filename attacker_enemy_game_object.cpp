#include "attacker_enemy_game_object.h"
#include <glm/gtc/constants.hpp>

namespace game {
	// Constructor
	AttackerEnemyGameObject::AttackerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
		intercept = false;
	}

	// handle shooting
	std::vector<Projectile*>* AttackerEnemyGameObject::Shoot(GLuint* textures) {
		std::vector<Projectile*>* vec = new std::vector<Projectile*>;

		// change to make sure player is in front as well (taken care of by checking that enemy is in pursuit/intercept)
		// && glm::length(target_->GetPosition() - position_) <= SHOOTING_RANGE old condition
		if (firing_timer_->Finished() && !exploding_ && (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
			firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

			// fire a basic projectile
			vec->push_back(new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, textures[tex_attacker_bullet], GetBearing(), player));
		}
		
		return vec;
	}
	void AttackerEnemyGameObject::Update(double delta_time, GLuint* textures)
	{
		if (stun_timer_->Finished()) {
			if (glm::length(target_->GetPosition() - position_) < PURSUIT_CHANGE_DISTANCE && glm::length(target_->GetPosition() - position_) > CHASE_CHANGE_DISTANCE) {
				state_ = intercepting;
			}
			// if patrolling, do the ellipse
			if (state_ == patrolling) {
				if (retarget_->Finished()) {
					// Random point in angle opening
					float r_num = ((float)rand()) / ((float)RAND_MAX);
					float opening = 5.0 * glm::pi<float>() / 180.0;
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
			// if intercepting, orbit the player
			else if (state_ == intercepting && !exploding_) {
				if (intercept == false) {
					intercept = true;
					glm::vec3 direction_to_player = target_->GetPosition() - position_;
					float angle_to_player = glm::atan(direction_to_player.y, direction_to_player.x);
					t_ = angle_to_player/(FULL_CIRCLE * rps_);
				}
				double rads = t_ * FULL_CIRCLE * rps_;
				position_ = glm::vec3(2 * cos(rads) + target_->GetPosition().x, 2 * sin(rads) + target_->GetPosition().y, 0.0);
				t_ += delta_time;
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
					heading = target_->GetPosition() - position_;
				}

				float angle = glm::atan(heading.y, heading.x);

				SetRotation(angle);

				//t_ += delta_time;
			}
		}

		GameObject::Update(delta_time, textures);
	}
}
