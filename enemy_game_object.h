#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"
#include "player_game_object.h"
#include "collectible_game_object.h"

#define FULL_CIRCLE 2 * 3.14159265358979323
#define RETARGET_LENGTH 2
#define PURSUIT_CHANGE_DISTANCE 2.0
#define CHASE_CHANGE_DISTANCE 1.5
#define ENEMY_FIRING_COOLDOWN 5

// definition for collectible drop rates
#define DROP_CHANCE 3

namespace game {
	// enemy's different states
	enum {
		patrolling = 0,
		pursuit = 1,
		intercepting = 2
	};

	// new class for enemy game objects
	class EnemyGameObject : public GameObject {
		public:
			// Constructor and Deconstructor
			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);
			virtual ~EnemyGameObject(void);

			// Setters
			void SetTarget(PlayerGameObject* player);

			// change the direction the enemy is travelling in
			void SetVelocityDirection(glm::vec3 velocity);

			// Update function for moving the enemy around
			void Update(double delta_time, GLuint* textures) override;

			// deal with collisions
			void Hit(GLuint* textures, GameObject* other) override;

			// handle shooting
			virtual Projectile* Shoot(GLuint* textures) = 0;

			// drop collectibles
			virtual CollectibleGameObject* DropCollectible(GLuint *textures);

		protected:
			// enemy movement state
			int state_;

			// ellipse attributes
			glm::vec3 ellipse_center_;
			float ellipse_width_;
			float ellipse_height_;
			float rps_;

			// interception attributes
			//glm::vec3 velocity_;
			PlayerGameObject *target_;
			Timer *retarget_;
			float speed_;
			glm::vec3 steering_;

			// time attribute for movement
			double t_;

			// attributes for shooting
			Timer *firing_timer_;
	};
}

#endif