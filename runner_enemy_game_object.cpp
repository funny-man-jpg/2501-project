#include "runner_enemy_game_object.h"
#include "emp_battery_collectible.h"
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
	void RunnerEnemyGameObject::Update(double delta_time, GLuint* textures)
	{
		if (glm::length(target_->GetPosition() - position_) < PURSUIT_CHANGE_DISTANCE && glm::length(target_->GetPosition() - position_) > CHASE_CHANGE_DISTANCE) {
			state_ = intercepting;
		}
		// if patrolling, do the ellipse
		if (state_ == patrolling) {
			/*
			double rads = t_ * FULL_CIRCLE * rps_;
			position_ = glm::vec3(ellipse_width_ * cos(rads) + ellipse_center_.x, ellipse_height_ * sin(rads) + ellipse_center_.y, 0.0);
			*/
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
		else if (state_ == intercepting) {
			steering_ =  position_ - target_->GetPosition() - velocity_;
			steering_ /= glm::length(steering_);
			steering_ *= 0.001;
			velocity_ += steering_;
		}
		EnemyGameObject::Update(delta_time, textures);
	}

	// chance to return a collectible
	CollectibleGameObject* RunnerEnemyGameObject::DropCollectible(GLuint* textures) {
		if (rand() % EMP_AMMO_DROP_CHANCE == 0) {
			return new EmpBatteryCollectible(position_, geometry_, shader_, textures[tex_emp_ammo]);
		}

		EnemyGameObject::DropCollectible(textures);
	}
}
