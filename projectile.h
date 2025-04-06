#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "game_object.h"

// give the projectile a lifespan
#define LIFESPAN 15

namespace game {
	class Projectile : public GameObject {
		public:
			// Constructor
			Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec3 direction, int target_type);

			// Getters
			int GetTargetType(void) const { return target_type_; }

			// Update function for moving the projectile
			virtual void Update(double delta_time, GLuint* textures) override;

			// function to check if 2 objects have collided (ray circle)
			virtual void CheckForCollision(GLuint *textures, GameObject* other) override;

			// function to handle collisions
			virtual void Hit(GLuint* textures, GameObject* other) override;

		protected:
			float speed_;
			glm::vec3 velocity_;

			// set up time trackers for the projectile
			float prev_time_;
			float curr_time_;

			// set up target trackers for the projectile
			GameObject *enemy_;
			float impact_time_;
			float soonest_impact_time_;

			// target type for the projectile
			int target_type_;
	};
}

#endif