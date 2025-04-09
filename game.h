#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "game_object.h"
#include "enemy_game_object.h"
#include "player_game_object.h"

// definition for the player's index in the game_objects_ vector
#define PLAYER 0

/*
// definition for the spaceship texture's index
#define SPACESHIP 1

// definitions for the two explosion texture indexes
#define EXPLOSION 4
#define BLOOD_SPLATTER 5
*/

// definition for the times between spawns
#define ATTACKER_SPAWN_TIME 10
#define PATROL_SPAWN_TIME 9
#define RUNNER_SPAWN_TIME 12
#define MIN_ENEMY_SPAWN_TIME 3
#define COLLECTIBLE_SPAWN_TIME 7

// game difficulty (how quickly enemies spawn as time passes) (1 second spawning deduction at the rate specified (in secs))
#define DIFFICULTY 30

// definitions for random number bounds
#define MAX_X 8000
#define MAX_Y 6000
#define DIVISOR 1000

// definition for random angles
#define MAX_DEGREES 360

// definition for spawning
#define SPAWN_SHIFT 1.0f

namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            Geometry* particles_;
            Geometry* explosion_;

            Shader particle_shader_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            Shader text_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // List of game objects
            std::vector<GameObject*> game_objects_;
            
            // player
            PlayerGameObject* player_;

            // Keep track of time
            double current_time_;
            double start_time_;

            // timers to track spawning times
            Timer *attacker_spawn_timer_;
            Timer *patrol_spawn_timer_;
            Timer *runner_spawn_timer_;
            Timer *invincibility_collectible_spawn_timer_;
            
            // a timer to track when to add to the score
            Timer score_timer_;

            // new attribute to determine if game is over
            bool game_over_;

            // new attribute to keep track of the user's score
            int *score_;

            // add a GameObject to the vector
            void AddGameObject(GameObject* obj);

            // Handle user input
            void HandleControls(double delta_time);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);

            // get a random position near to the player
            glm::vec3 GetRandomPosition();

            // get a random angle between 0 and 2*pi
            float GetRandomAngle();

            // create a new enemy
            void SpawnNewEnemy(EnemyGameObject* enemy);
    }; // class Game

} // namespace game

#endif // GAME_H_
