#include "runner_enemy_game_object.h"
#include "emp_battery_collectible.h"
#include "bomb.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {
	// Constructor
	RunnerEnemyGameObject::RunnerEnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
	}

	// handle shooting
	std::vector<Projectile*>* RunnerEnemyGameObject::Shoot(GLuint* textures) {
		std::vector<Projectile*>* vec = new std::vector<Projectile*>;

		// change to make sure player is behind (taken care of by checking that enemy is running away)
		if (firing_timer_->Finished() && !exploding_ && (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
			firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

			// drop a bomb
			vec->push_back(new Bomb(position_ - radius_ * GetBearing(), geometry_, shader_, textures[tex_bomb], GetBearing(), player));
		}

		return vec;
	}

	void RunnerEnemyGameObject::Update(double delta_time, GLuint* textures)
	{
		if (stun_timer_->Finished()) {
			if (glm::length(target_->GetPosition() - position_) < PURSUIT_CHANGE_DISTANCE && glm::length(target_->GetPosition() - position_) > CHASE_CHANGE_DISTANCE) {
				state_ = intercepting;
			}
			// if patrolling, do the ellipse
			if (state_ == patrolling) {
				/*
				double rads = t_ * FULL_CIRCLE * rps_;
				position_ = glm::vec3(ellipse_width_ * cos(rads) + ellipse_center_.x, ellipse_height_ * sin(rads) + ellipse_center_.y, 0.0);
				*/
				if (retarget_->Finished()) {
					// Random point in angle opening
					float r_num = ((float)rand()) / ((float)RAND_MAX);
					float opening = 5.0 * glm::pi<float>() / 180.0;
					float r_angle = r_num * 2.0 * opening + angle_ - opening;
					float r = 0.25;
					glm::vec3 target(r * cos(r_angle), r * sin(r_angle), 0.0);
					// Steering to target
					glm::vec3 desired = target;
					glm::vec3 steering = desired + velocity_;
					steering /= glm::length(steering);
					steering *= 0.1; // Adjust force magnitude
					velocity_ += steering;
					// Reset timer to only update wander every 1s
					retarget_->Start(1.0);
				}

			}
			// if intercepting, chase the player
			else if (state_ == intercepting) {
				steering_ = position_ - target_->GetPosition() - velocity_;
				steering_ /= glm::length(steering_);
				steering_ *= 0.001;
				velocity_ += steering_;
				float mag = glm::length(velocity_);
				if (mag > 2.5f) {
					velocity_.x = (velocity_.x / mag) * 2.0f;
					velocity_.y = (velocity_.y / mag) * 2.0f;
				}
			}
		}

		EnemyGameObject::Update(delta_time, textures);
	}

	// chance to return a collectible
	CollectibleGameObject* RunnerEnemyGameObject::DropCollectible(GLuint* textures) {
		if (rand() % EMP_AMMO_DROP_CHANCE == 0) {
			return new EmpBatteryCollectible(position_, geometry_, shader_, textures[tex_emp_ammo]);
		}

		EnemyGameObject::DropCollectible(textures);
	}
	void RunnerEnemyGameObject::Render(glm::mat4 view_matrix, double current_time) {

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

		glm::mat4 global = translation_matrix * rotation_matrix;

		// Setup the transformation matrix for the shader

		glm::mat4 transformation_matrix = global * scaling_matrix;

		/*
		if (segments > 1) {
			child->Render(view_matrix, current_time, global);
		}
		*/

		// Set the transformation matrix in the shader
		shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

		// Set up the geometry
		geometry_->SetGeometry(shader_->GetShaderProgram());

		// Bind the entity's texture
		glBindTexture(GL_TEXTURE_2D, texture_);

		// Draw the entity
		glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
	}
	void RunnerEnemyGameObject::Render(glm::mat4 view_matrix, double current_time, glm::mat4 local)
	{
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

		glm::mat4 local_transformation = local * translation_matrix * rotation_matrix;

		// Setup the transformation matrix for the shader

		glm::mat4 transformation_matrix = local_transformation * scaling_matrix;

		/*
		if (segments > 1) {
			child->Render(view_matrix, current_time, local_transformation);
		}
		*/

		// Set the transformation matrix in the shader
		shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

		// Set up the geometry
		geometry_->SetGeometry(shader_->GetShaderProgram());

		// Bind the entity's texture
		glBindTexture(GL_TEXTURE_2D, texture_);

		// Draw the entitys
		glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
	}
}
