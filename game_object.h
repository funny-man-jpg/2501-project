#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"

// add the timer header
#include "timer.h"

// define the explosion length
#define EXPLOSION_LENGTH 5

namespace game {
    // game object types
    enum {
        player = 0,
        collectible = 1,
        enemy = 2,
        normal = 3,
        projectile = 4
    };

    // game object textures
    enum { 
        tex_purple_dragon = 0,
        tex_spaceship = 1,
        tex_stars = 2,
        tex_orb = 3,
        tex_explosion = 4,
        tex_blood_splatter = 5,
        tex_star_collectible = 6,
        tex_purple_dragon_invincible = 7,
        tex_fireball = 8,
        tex_font = 9
    };

    // collision types
    enum {
        circle = 0,
        ray = 1
    };

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor and Deconstructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);
            virtual ~GameObject(void);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time, GLuint *textures);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }
            bool GetCollideability(void) const { return collideable_;  }
            bool GetDelete(void) const { return delete_; }
            bool GetExploding(void) const { return exploding_; }
            int GetType(void) const { return type_; }
            float GetRadius(void) const { return radius_; }
            glm::vec3 GetVelocity(void) const { return velocity_; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(glm::vec2 scale) { scale_ = scale; }
            void SetRotation(float angle);

            // new setter to set the texture size of the game object
            void SetTexSize(float size) { tex_size_ = size; }

            // new Setter to set whether the game object can be collided with or not
            void SetCollideability(bool collideable);

            // new function that handles the collision of a game object
            virtual void Hit(GLuint *textures, GameObject *other);

            // new function to check if 2 objects have collided
            virtual void CheckForCollision(GLuint *textures, GameObject *other);


        protected:
            // Object's Transform Variables
            glm::vec3 velocity_;
            glm::vec3 position_;
            glm::vec2 scale_;
            float angle_;

            // Geometry
            Geometry *geometry_;

            // new variable to tell whether a game object is collideable or not
            bool collideable_;

            // new variables for explosion timer
            bool exploding_;
            Timer *timer_;

            // new variable to determine if an object is to be deleted
            bool delete_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

            // object's type
            int type_;

            // object's radius (for circle to circle collisions)
            float radius_;

            // object's collision type
            int collision_type_;

            // ghost property
            int ghost_;

            // texture size
            float tex_size_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
