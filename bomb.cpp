#include "bomb.h"

namespace game {
	// Constructor
	Bomb::Bomb(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec3 direction, int target_type) 
		: Projectile(position, geom, shader, texture, direction, target_type) {
		// set the collision type to circle circle
		collision_type_ = circle;

		// change the scale
		scale_ = glm::vec2(0.25f);
	}

	// Update function for moving the projectile
	void Bomb::Update(double delta_time, GLuint* textures) {
		if (timer_->Finished()) {
			if (exploding_) {
				delete_ = true;
			}
			else {
				// change the explosion radius
				radius_ = 0.5;

				// check if explosion hits player (not working)
				//CheckForCollision(textures, enemy_);

				// make sure to explode
				if (!exploding_) {
					// doesnt work
					//Hit(textures, enemy_);

					SetCollideability(false);
					texture_ = textures[tex_explosion];
					exploding_ = true;
					scale_ = glm::vec2(1.25f);
					timer_->Start(EXPLOSION_LENGTH);
				}
			}
		}
	}

	// function to check if 2 objects have collided (circle circle)
	void Bomb::CheckForCollision(GLuint* textures, GameObject* other) {
		if (other->GetType() == target_type_ || other->GetType() == emp_ring) {
			GameObject::CheckForCollision(textures, other);
		}
	}

	// function to handle collisions
	void Bomb::Hit(GLuint* textures, GameObject* other) {
		if (other->GetType() == target_type_ || other->GetType() == emp_ring) {
			SetCollideability(false);
			texture_ = textures[tex_explosion];
			exploding_ = true;
			scale_ = glm::vec2(1.25f);
			timer_->Start(EXPLOSION_LENGTH);
		}
	}
}