#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

// add library for random numbers
#include <cstdlib>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
// add the collectible game object header
#include "collectible_game_object.h"
// add the enemy game object header
#include "enemy_game_object.h"
// add the Projectile class header
#include "projectile.h"
// add the new enemy type headers
#include "attacker_enemy_game_object.h"
#include "patrol_enemy_game_object.h"
#include "runner_enemy_game_object.h"
#include "game.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;

    // changed the previous player and enemy textures into these two
    textures.push_back("/textures/purple_dragon.png"); 
    textures.push_back("/textures/spaceship.png");

    textures.push_back("/textures/stars.png");
    textures.push_back("/textures/orb.png");

    // add the two explosion textures
    textures.push_back("/textures/explosion.png");
    textures.push_back("/textures/blood_splatter.png");

    // add the collectible/invincibility textures
    textures.push_back("/textures/star_collectible.png");
    textures.push_back("/textures/purple_dragon_invincible.png");

    // add the projectile texture
    textures.push_back("/textures/fireball.png");

    // Load textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector
    player_ = new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_purple_dragon]);
    game_objects_.push_back(player_);
    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[0]->SetRotation(pi_over_two);

    // Setup other objects
    EnemyGameObject *enemy1 = new EnemyGameObject(glm::vec3(-3.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_spaceship], player_);
    game_objects_.push_back(enemy1);
    game_objects_[1]->SetRotation(pi_over_two);
    // give the enemy a reference to the player
    //enemy1->SetTarget(player_);
    EnemyGameObject* enemy2 = new EnemyGameObject(glm::vec3(1.0f, -2.75f, 0.0f), sprite_, &sprite_shader_, tex_[tex_spaceship], player_);
    game_objects_.push_back(enemy2);
    game_objects_[2]->SetRotation(pi_over_two);
    // give the enemy a reference to the player
    //enemy2->SetTarget(player_);
    EnemyGameObject* enemy3 = new EnemyGameObject(glm::vec3(4.0f, 5.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_spaceship], player_);
    game_objects_.push_back(enemy3);
    game_objects_[3]->SetRotation(pi_over_two);
    // give the enemy a reference to the player
    //enemy3->SetTarget(player_);
    EnemyGameObject* enemy4 = new EnemyGameObject(glm::vec3(-2.2f, -3.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_spaceship], player_);
    game_objects_.push_back(enemy4);
    game_objects_[4]->SetRotation(pi_over_two);
    // give the enemy a reference to the player
    //enemy4->SetTarget(player_);

    // Setup collectible objects
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, -2.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_star_collectible]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-2.6f, 1.75f, 0.0f), sprite_, &sprite_shader_, tex_[tex_star_collectible]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(4.5f, 1.2f, 0.0f), sprite_, &sprite_shader_, tex_[tex_star_collectible]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-4.7f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_star_collectible]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(2.5f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_star_collectible]));

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_stars]);

    // make the background 10 times bigger than normal
    background->SetScale(glm::vec2(120.0f));

    // set the background's texture size to 10
    background->SetTexSize(10.0f);

    // make sure the background can not be collided with
    background->SetCollideability(false);

    game_objects_.push_back(background);
}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }
}

void Game::AddGameObject(GameObject* obj) {
    if (obj != nullptr) {
        // used cplusplus.com standard vector/insert documentation
        game_objects_.insert(game_objects_.end() - 1, obj);
    }
}


void Game::HandleControls(double delta_time)
{
    // Get player game object
    GameObject *player = game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*500.0;
    float motion_increment = 0.001*speed;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        // accelerate forwards
        player_->AccelForwards(delta_time);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        // accelerate backwards
        player_->AccelBackwards(delta_time);
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        // accelerate right
        player_->AccelRight(delta_time);
    }
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        // accelerate left
        player_->AccelLeft(delta_time);
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        AddGameObject(player_->Fire(tex_[tex_fireball]));
    }
    /*
    if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
        player->SetPosition(curpos - motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
        player->SetPosition(curpos + motion_increment*player->GetRight());
    }
    */
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
}


void Game::Update(double delta_time)
{
    // setup the counter
    int i = 0;

    // Update all game objects
    // use a while loop to let the removal of objects from game_objects_ work
    while (i < game_objects_.size()) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time, tex_);

        // check if the object should be deleted
        if (current_game_object->GetDelete()) {
            // if the deleted object is the player, game is over
            if (current_game_object->GetType() == player) {
                game_over_ = true;
            }

            delete current_game_object;

            // used cplusplus.com vector/erase documentation to help figure out how to do this
            game_objects_.erase(game_objects_.begin() + i);
            continue;
        }

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world

        // only check for collisions with objects that can be collided with
        if (current_game_object->GetCollideability() == true) {
            for (int j = i + 1; j < (game_objects_.size() - 1); j++) {
                GameObject* other_game_object = game_objects_[j];
                
                // only check for collisions with objects that can be collided with
                if (other_game_object->GetCollideability() == true) {
                    current_game_object->CheckForCollision(tex_, other_game_object);
                }
            }
        }

        // increment the counter
        i++;
    }

    // check for adding of new enemies
    if (spawn_timer_->Finished() && !game_objects_[PLAYER]->GetExploding() && !game_over_) {
        float x, y;

        // get random position
        // used w3schools C++ How To Generate Random Numbers
        x = (float) (rand() % MAX_X) / DIVISOR;
        y = (float) (rand() % MAX_Y) / DIVISOR;

        if (rand() % 2) {
            x = -x;
        }

        if (rand() % 2) {
            y = -y;
        }

        // spawn enemy
        EnemyGameObject* new_enemy = new EnemyGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, tex_[SPACESHIP], player_);
        //new_enemy->SetTarget(player_);
        AddGameObject(new_enemy);

        spawn_timer_->Start(SPAWN_TIME);
    }
}


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));

    // move the camera to center on the player
    glm::mat4 camera_translate_matrix = glm::translate(glm::mat4(1.0f), -(player_->GetPosition()));

    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * camera_translate_matrix;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        // only let the player move if they're still alive
        if (!game_objects_[PLAYER]->GetExploding()) {
            HandleControls(delta_time);
        }

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // check if game is over
        if (game_over_) {
            // used glfw.org Window documentation
            glfwSetWindowShouldClose(window_, true);
            std::cout << "Game Over!" << std::endl;
        }
    }
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
    start_time_ = 0.0;

    // initialize spawn_timer_
    spawn_timer_ = new Timer();
    spawn_timer_->Start(SPAWN_TIME);

    // make sure random numbers are different
    // used w3schools C++ How To Generate Random Numbers
    srand(time(0));

    // set the player to a nullptr
    player_ = nullptr;

    // set game_over_ to false
    game_over_ = false;

    // initialize the score
    score_ = 0;
}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

} // namespace game
