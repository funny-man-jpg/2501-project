#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"
#include "player_game_object.h"

#define FULL_CIRCLE 2 * 3.14159265358979323
#define RETARGET_LENGTH 2
#define STATE_CHANGE_DISTANCE 2.0

namespace game {
	// enemy's different states
	enum {
		patrolling = 0,
		intercepting = 1
	};

	// new class for enemy game objects
	class EnemyGameObject : public GameObject {
		public:
			// Constructor and Deconstructor
			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
			~EnemyGameObject(void);

			// Setters
			void SetTarget(PlayerGameObject* player);

			// Update function for moving the enemy around
			void Update(double delta_time, GLuint* textures) override;

			// deal with collisions
			void Hit(GLuint* textures, GameObject* other) override;

		private:
			// patrolling or intercepting
			bool state_;

			// ellipse attributes
			glm::vec3 ellipse_center_;
			float ellipse_width_;
			float ellipse_height_;
			float rps_;

			// interception attributes
			glm::vec3 velocity_;
			PlayerGameObject *target_;
			Timer *retarget_;
			float speed_;

			// time attribute for movement
			double t_;
	};
}

#endif