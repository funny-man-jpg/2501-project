#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "game_object.h"

// add the timer header
#include "timer.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) 
{

    // Initialize all attributes
    position_ = position;
    scale_ = glm::vec2(1.0f);
    angle_ = 0.0;
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    radius_ = 0.4;

    // set the game object to collideable by default
    collideable_ = true;

    // set objects to not exploding by default
    exploding_ = false;

    // set up the timer
    timer_ = new Timer();

    // set objects to not to be deleted by default
    delete_ = false;

    // set type to normal object
    type_ = normal;

    // set collision type
    collision_type_ = circle;

    // rotate in the correct direction
    SetRotation(glm::pi<float>() / 2.0f);

    // turn off ghost mode by default
    ghost_ = 0;

    // set texture size to 1 by default
    tex_size_ = 1;
}

// Deconstructor
GameObject::~GameObject(void) {
    delete timer_;
}


glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return dir;
}

void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}

// implement the collideable_ setter
void GameObject::SetCollideability(bool collideable) {
    // set the collideable_ variable to reflect if the object can be collided with
    collideable_ = collideable;
}


void GameObject::Update(double delta_time, GLuint *textures) {
    // if the object is exploding and the timer is up, set the object to be deleted
    if (exploding_ && timer_->Finished()) {
        delete_ = true;
    }
}


void GameObject::Render(glm::mat4 view_matrix, double current_time){

    // Set up the shader
    shader_->Enable();

    // Tell the shader if ghost mode is on or off
    shader_->SetUniform1i("ghost", ghost_);

    // tell the shader how big the texture is
    shader_->SetUniform1f("tex_size", tex_size_);

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

// new method to deal with game object collisions
void GameObject::Hit(GLuint *textures, GameObject *other) {
    // does nothing
}

// new function to check if 2 objects have collided
void GameObject::CheckForCollision(GLuint *textures, GameObject* other) {
    // check for collision type
    if (other->collision_type_ == ray) {
        other->CheckForCollision(textures, this);
    }
    else {
        // do circle to circle collision
        float distance = glm::length(this->position_ - other->position_);

        if (distance < this->radius_ + other->radius_) {
            this->Hit(textures, other);
            other->Hit(textures, this);
        }
    }
}

} // namespace game
