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
	collected_ = 0;
	invincible_timer_ = new Timer();
	invincible_ = false;

	velocity_ = glm::vec3(0.0, 0.0, 0.0);
	accel_mag_ = 0.5;

	firing_timer_ = new Timer();
}

// Deconstructor
PlayerGameObject::~PlayerGameObject(void) {
	delete invincible_timer_;
	delete timer_;
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

		// update the player's position based on velocity
		float new_x = position_.x + velocity_.x * delta_time;
		float new_y = position_.y + velocity_.y * delta_time;
		position_ = glm::vec3(new_x, new_y, 0.0);
	}
	
	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time, textures);
}

// acceleration functions
void PlayerGameObject::AccelForwards(double delta_time) {
	velocity_ += GetBearing() * (float)(accel_mag_ * delta_time);

	// clamp the velocity
	if (glm::length(velocity_) > MAX_VELOCITY) {
		velocity_ = (velocity_ / glm::length(velocity_)) * (float) MAX_VELOCITY;
	}
}

void PlayerGameObject::AccelBackwards(double delta_time) {
	velocity_ -= GetBearing() * (float)(accel_mag_ * delta_time);

	// clamp the velocity
	if (glm::length(velocity_) > MAX_VELOCITY) {
		velocity_ = (velocity_ / glm::length(velocity_)) * (float)MAX_VELOCITY;
	}
}

void PlayerGameObject::AccelRight(double delta_time) {
	velocity_ += GetRight() * (float)(accel_mag_ * delta_time);

	// clamp the velocity
	if (glm::length(velocity_) > MAX_VELOCITY) {
		velocity_ = (velocity_ / glm::length(velocity_)) * (float)MAX_VELOCITY;
	}
}

void PlayerGameObject::AccelLeft(double delta_time) {
	velocity_ -= GetRight() * (float)(accel_mag_ * delta_time);

	// clamp the velocity
	if (glm::length(velocity_) > MAX_VELOCITY) {
		velocity_ = (velocity_ / glm::length(velocity_)) * (float)MAX_VELOCITY;
	}
}

// function to shoot a projectile
Projectile* PlayerGameObject::Fire(GLuint tex) {
	if (firing_timer_->Finished()) {
		firing_timer_->Start(FIRING_COOLDOWN);

		// make a new projectile directly in front of the player
		return new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, tex, GetBearing());
	}

	return nullptr;
}

// new function to handle the player collisions
void PlayerGameObject::Hit(GLuint *textures, GameObject *other) {
	// for colliding with EnemyGameObjects
	if (other->GetType() == enemy && !invincible_) {
		health_--;

		if (health_ == 0) {
			SetCollideability(false);
			texture_ = textures[tex_blood_splatter];
			exploding_ = true;
			timer_->Start(EXPLOSION_LENGTH);
		}
	}
	// for colliding with CollectibleGameObjects
	else if (other->GetType() == collectible) {
		collected_++;

		if (collected_ == MAX_COLLECTIBLES) {
			invincible_ = true;
			collected_ = 0;
			texture_ = textures[tex_purple_dragon_invincible];
			invincible_timer_->Start(INVINCIBILITY_LENGTH);
		}
	}
}

} // namespace game
