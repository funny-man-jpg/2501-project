#include "enemy_game_object.h"
#include "health_collectible.h"

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

		// set up shooting
		firing_timer_ = new Timer();
		firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

		// set up stun timer
		stun_timer_ = new Timer();
	}

	// Deconstructor
	EnemyGameObject::~EnemyGameObject(void) {
		delete retarget_;
		delete firing_timer_;
		delete stun_timer_;
	}

	// Setters
	void EnemyGameObject::SetTarget(PlayerGameObject* player) {
		target_ = player;
	}

	// change the direction the enemy is travelling in
	void EnemyGameObject::SetVelocityDirection(glm::vec3 velocity) { 
		velocity_ = (velocity / glm::length(velocity)) * speed_; 
	}

	// Update function for moving the enemy around
	void EnemyGameObject::Update(double delta_time, GLuint* textures) {
		if (stun_timer_->Finished()) {
			// only update the position if still alive/not exploding
			if (!exploding_) {
				glm::vec3 prev_position = position_;

				// check if need to change state


				position_ += velocity_ * (float)delta_time;
				glm::vec3 heading = position_ - prev_position;

				float angle = glm::atan(heading.y, heading.x);
				SetRotation(angle);

				t_ += delta_time;
			}
		}

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time, textures);
	}

	void EnemyGameObject::Hit(GLuint* textures, GameObject* other) {
		if (other->GetType() == player || (other->GetType() == projectile && ((Projectile*) other)->GetTargetType() == enemy)) {
			SetCollideability(false);
			texture_ = textures[tex_explosion];
			exploding_ = true;
			timer_->Start(EXPLOSION_LENGTH);
		}
		else if (other->GetType() == emp_ring) {
			stun_timer_->Start(STUN_DURATION);
		}
	}

	// chance to return a collectible
	CollectibleGameObject* EnemyGameObject::DropCollectible(GLuint* textures) {
		if (rand() % DROP_CHANCE == 0) {
			return new HealthCollectible(position_, geometry_, shader_, textures[tex_heart]);
		}

		return nullptr;
	}
}