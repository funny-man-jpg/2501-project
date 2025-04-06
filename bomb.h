#ifndef BOMB_H_
#define BOMB_H_

#include "projectile.h"

namespace game {
	class Bomb : public Projectile {
		public:
			// Constructor
			Bomb(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec3 direction, int target_type);

			// Update function for moving the projectile
			virtual void Update(double delta_time, GLuint* textures);

			// function to check if 2 objects have collided (circle circle)
			virtual void CheckForCollision(GLuint* textures, GameObject* other);

			// function to handle collisions
			virtual void Hit(GLuint* textures, GameObject* other);
	};
}

#endif