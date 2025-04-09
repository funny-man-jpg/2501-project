#include "player_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
	// set health to 3
	health_ = 3;

	type_ = player;

	emp_battery_collected_ = 0;
	invincibility_collected_ = 0;
	invincible_timer_ = new Timer();
	invincible_ = false;

	velocity_ = glm::vec3(0.0, 0.0, 0.0);
	accel_mag_ = 3.0f;

	firing_timer_ = new Timer();
	emp_ring_timer_ = new Timer();
}

// Deconstructor
PlayerGameObject::~PlayerGameObject(void) {
	delete invincible_timer_;
	delete firing_timer_;
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time, GLuint *textures) {
	if (!exploding_) {
		// check if invincibility is over
		if (invincible_ == true && invincible_timer_->Finished()) {
			invincible_ = false;
			texture_ = textures[tex_purple_dragon];
		}

		//Getting the magnitude to clamp movement
		float mag = glm::length(velocity_);

		//Adding friction because I want my ship to decelerate
		/*
		float friction = 0.001f;
		if (mag > 0)
		{
			float newMagnitude = mag - friction;
			//preventing going backwards from friction
			if (newMagnitude < 0) newMagnitude = 0;
			//lowering the magnitude, we can still move since the acceleration beats out the friction when we do press buttons
			velocity_.x = (velocity_.x / mag) * newMagnitude;
			velocity_.y = (velocity_.y / mag) * newMagnitude;
		}
		*/
		//clamping the magnitude to 2
		if (mag > 2.0f) {
			velocity_.x = (velocity_.x / mag) * 2.0f;
			velocity_.y = (velocity_.y / mag) * 2.0f;
		}

		// update the player's position based on velocity

		position_ += velocity_ * (float)delta_time;
		/*
		float new_x = position_.x + velocity_.x * delta_time;
		float new_y = position_.y + velocity_.y * delta_time;
		
		position_ = glm::vec3(new_x, new_y, 0.0);
		*/
	}
	
	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time, textures);
}

// acceleration functions
void PlayerGameObject::AccelForwards(double delta_time) {
	velocity_ += GetBearing() * (float)(accel_mag_ * delta_time);
}

void PlayerGameObject::AccelBackwards(double delta_time) {
	velocity_ -= GetBearing() * (float)(accel_mag_ * delta_time);
}

void PlayerGameObject::AccelRight(double delta_time) {
	velocity_ += GetRight() * (float)(accel_mag_ * delta_time);
}

void PlayerGameObject::AccelLeft(double delta_time) {
	velocity_ -= GetRight() * (float)(accel_mag_ * delta_time);
}

// function to shoot a projectile
Projectile* PlayerGameObject::Fire(GLuint tex) {
	if (firing_timer_->Finished()) {
		firing_timer_->Start(FIRING_COOLDOWN);

		// make a new projectile directly in front of the player
		return new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, tex, GetBearing(), enemy);
	}

	return nullptr;
}

// second weapon shooting function (emp ring)
EmpRing* PlayerGameObject::EmpRingFire(GLuint tex) {
	if (emp_battery_collected_ > 0 && emp_ring_timer_->Finished()) {
		emp_battery_collected_--;
		emp_ring_timer_->Start(EMP_RING_DURATION);

		return new EmpRing(position_, geometry_, shader_, tex, this);
	}

	return nullptr;
}

// new function to handle the player collisions
void PlayerGameObject::Hit(GLuint *textures, GameObject *other) {
	// for colliding with EnemyGameObjects
	if ((other->GetType() == enemy || other->GetType() == projectile) && !invincible_) {
		health_--;

		if (health_ == 0) {
			SetCollideability(false);
			texture_ = textures[tex_blood_splatter];
			exploding_ = true;
			timer_->Start(EXPLOSION_LENGTH);
		}
	}
	// for colliding with CollectibleGameObjects
	else if (other->GetType() == invincibility_collectible) {
		invincibility_collected_++;

		if (invincibility_collected_ == MAX_INVINCIBILITY_COLLECTIBLES) {
			invincible_ = true;
			invincibility_collected_ = 0;
			texture_ = textures[tex_purple_dragon_invincible];
			invincible_timer_->Start(INVINCIBILITY_LENGTH);
		}
	}
	// for colliding with HealthCollectibles
	else if (other->GetType() == health_collectible) {
		if (health_ < MAX_HEALTH) {
			health_++;
		}
	}
	// for colliding with EmpBatteryCollectibles
	else if (other->GetType() == emp_battery_collectible) {
		if (emp_battery_collected_ < MAX_EMP_BATTERY) {
			emp_battery_collected_++;
		}
	}
}

} // namespace game
