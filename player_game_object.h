#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
// add the Projectile class header
#include "projectile.h"

// definitions for collectibles and invincibility
#define INVINCIBILITY_LENGTH 10
#define MAX_COLLECTIBLES 5

// motion definitions
#define MAX_VELOCITY 2

// firing definitions
#define FIRING_COOLDOWN 3

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            // Constructor and Deconstructor
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);
            virtual ~PlayerGameObject(void);

            // Update function for moving the player object around
            void Update(double delta_time, GLuint *textures) override;

            // new function to handle the player colliding
            void Hit(GLuint *textures, GameObject *other) override;

            // acceleration functions
            void AccelForwards(double delta_time);
            void AccelBackwards(double delta_time);
            void AccelRight(double delta_time);
            void AccelLeft(double delta_time);

            // shooting function
            Projectile* Fire(GLuint tex);

        private:
            // a new attribute for the player's health
            int health_;

            // number of collectible objects collected
            int collected_;

            // whether the player is invincible at the moment or not
            bool invincible_;

            // timer for invincibility
            Timer *invincible_timer_;

            // motion values
            float accel_mag_;
            glm::vec3 velocity_;

            // firing values
            Timer *firing_timer_;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
