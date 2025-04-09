#ifndef WAVE_PROJECTILE_H_
#define WAVE_PROJECTILE_H_

#include "projectile.h"

#define SINE_WAVE_HEIGHT 0.001f

namespace game {
	class WaveProjectile : public Projectile {
		public:
			// Constructor
			WaveProjectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec3 direction, int target_type, bool right);

			// Update function for moving the projectile
			virtual void Update(double delta_time, GLuint* textures);

			// function to check if 2 objects have collided (circle circle)
			virtual void CheckForCollision(GLuint* textures, GameObject* other);

			// function to handle collisions
			virtual void Hit(GLuint* textures, GameObject* other);

		private:
			// culmulative time for movement
			double culmulative_time_;

			// up direction
			glm::vec3 up_direction_;
	};
}

#endif