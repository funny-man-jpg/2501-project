#include "wave_projectile.h"

namespace game {
	// Constructor
	WaveProjectile::WaveProjectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec3 direction, int target_type)
		: Projectile(position, geom, shader, texture, direction, target_type) {
		// set the collision type to circle circle
		collision_type_ = circle;

		// make the projectile slow
		speed_ = 1.5f;

		// set culmulative time to 0
		culmulative_time_ = 0;

		// change the scale
		scale_ = glm::vec2(0.25f);
	}

	// Update function for moving the projectile
	void WaveProjectile::Update(double delta_time, GLuint* textures) {
		culmulative_time_ += delta_time;

		if (!exploding_) {
			// move forward at a constant rate
			glm::vec3 forwards = GetBearing();
			forwards = forwards / glm::length(forwards);
			position_ += forwards * speed_ * (float)delta_time;

			// move up and down based on the sine function (doesnt work rn, moves up and down wayyyy too much)
			glm::vec3 up = -GetRight();
			up = up / glm::length(up);
			up = up * (float)glm::sin(culmulative_time_ * speed_) * radius_;
			position_ += up;
		}

		if (timer_->Finished()) {
			delete_ = true;
		}
	}

	// function to check if 2 objects have collided (circle circle)
	void WaveProjectile::CheckForCollision(GLuint* textures, GameObject* other) {
		if (other->GetType() == target_type_ || other->GetType() == emp_ring) {
			GameObject::CheckForCollision(textures, other);
		}
	}

	// function to handle collisions
	void WaveProjectile::Hit(GLuint* textures, GameObject* other) {
		if (other->GetType() == target_type_ || other->GetType() == emp_ring) {
			SetCollideability(false);
			texture_ = textures[tex_explosion];
			exploding_ = true;
			scale_ = glm::vec2(0.75f);
			timer_->Start(EXPLOSION_LENGTH);
		}
	}
}