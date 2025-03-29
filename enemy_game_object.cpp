#include "enemy_game_object.h"

namespace game {
	// Constructor
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
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
		target_ = nullptr;
		retarget_ = new Timer();
		speed_ = 0.25;
	}

	// Deconstructor
	EnemyGameObject::~EnemyGameObject(void) {
		delete timer_;
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
			if (glm::length(target_->GetPosition() - position_) < STATE_CHANGE_DISTANCE) {
				state_ = intercepting;
			}

			// if patrolling, do the ellipse
			if (state_ == patrolling) {
				double rads = t_ * FULL_CIRCLE * rps_;
				position_ = glm::vec3(ellipse_width_ * cos(rads) + ellipse_center_.x, ellipse_height_ * sin(rads) + ellipse_center_.y, 0.0);
			}
			// if intercepting, chase the player
			else if (state_ == intercepting) {
				// when timer is up, decide on new target
				if (retarget_->Finished()) {
					velocity_ = target_->GetPosition() - position_;
					velocity_ = velocity_ / glm::length(velocity_) * speed_;
					retarget_->Start(RETARGET_LENGTH);
				}

				// head to target using velocity
				float new_x = position_.x + velocity_.x * delta_time;
				float new_y = position_.y + velocity_.y * delta_time;
				position_ = glm::vec3(new_x, new_y, 0.0);
			}

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