#include "enemy_game_object.h"

namespace game {
	// Constructor
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: GameObject(position, geom, shader, texture) {
		type_ = enemy;
		state_ = patrolling;
		t_ = 0;

		// set up the ellipse
		ellipse_center_ = position;
		ellipse_width_ = 0.75;
		ellipse_height_ = 1.25;
		rps_ = 0.1;

		// set up interception
		velocity_ = glm::vec3(0.0, 0.0, 0.0);
		target_ = target;
		retarget_ = new Timer();
		retarget_->Start(0.01f);
		speed_ = 0.25;
	}

	// Deconstructor
	EnemyGameObject::~EnemyGameObject(void) {
		delete retarget_;
	}

	// Setters
	void EnemyGameObject::SetTarget(PlayerGameObject* player) {
		target_ = player;
	}

	// Update function for moving the enemy around
	void EnemyGameObject::Update(double delta_time, GLuint* textures) {
		// only update the position if still alive/not exploding
		if (!exploding_) {
			glm::vec3 prev_position = position_;

			// check if need to change state
			if (glm::length(target_->GetPosition() - position_) < PURSUIT_CHANGE_DISTANCE && glm::length(target_->GetPosition() - position_) > CHASE_CHANGE_DISTANCE) {
				state_ = pursuit;
			}else if (glm::length(target_->GetPosition() - position_) < CHASE_CHANGE_DISTANCE) {
				state_ = intercepting;
			}

			// if patrolling, do the ellipse
			if (state_ == patrolling) {
				/*
				double rads = t_ * FULL_CIRCLE * rps_;
				position_ = glm::vec3(ellipse_width_ * cos(rads) + ellipse_center_.x, ellipse_height_ * sin(rads) + ellipse_center_.y, 0.0);
				*/
				if (retarget_->Finished()){
					// Random point in angle opening
					float r_num = ((float)rand()) / ((float)RAND_MAX);
					float opening = 5.0*3.141592/180.0; // Add PI from the glm library
					float r_angle = r_num*2.0*opening + angle_ - opening;
					float r = 0.25;
					glm::vec3 target(r*cos(r_angle), r*sin(r_angle), 0.0);
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
			else if (state_ == pursuit) {
				glm::vec3 futurePosition = target_->GetPosition() + target_->GetVelocity() * 1.0f;

				steering_ = futurePosition - position_ - velocity_;
				steering_ /= glm::length(steering_);
				steering_ *= 0.001;
				velocity_ += steering_;
				// when timer is up, decide on new target
				/*
				if (retarget_->Finished()) {
					velocity_ = target_->GetPosition() - position_;
					velocity_ = velocity_ / glm::length(velocity_) * speed_;
					retarget_->Start(RETARGET_LENGTH);
				}

				// head to target using velocity
				float new_x = position_.x + velocity_.x * delta_time;
				float new_y = position_.y + velocity_.y * delta_time;
				position_ = glm::vec3(new_x, new_y, 0.0);
				*/
				
			}
			else if (state_ == intercepting) {

				steering_ = target_->GetPosition() - position_ - velocity_;
				steering_ /= glm::length(steering_);
				steering_ *= 0.001;
				velocity_ += steering_;
			}
			position_ += velocity_ * (float)delta_time;
			glm::vec3 heading = position_ - prev_position;

			float angle = glm::atan(heading.y, heading.x);
			SetRotation(angle);

			t_ += delta_time;
		}

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time, textures);
	}

	void EnemyGameObject::Hit(GLuint* textures, GameObject* other) {
		if (other->GetType() == player || other->GetType() == projectile) {
			SetCollideability(false);
			texture_ = textures[tex_explosion];
			exploding_ = true;
			timer_->Start(EXPLOSION_LENGTH);
		}
	}
}