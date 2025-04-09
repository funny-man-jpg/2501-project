#include "boss_enemy_game_object.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {
	// Constructor
	BossEnemyObject::BossEnemyObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
		base_part_ = new BossPart(position, geom, shader, texture, target);
		gun_arm_ = new BossPart(position, geom, shader, texture, target);
		barrel_ = new BossPart(position, geom, shader, texture, target);

		base_part_->AddChild(gun_arm_);
		gun_arm_->AddChild(barrel_);
	}

	// handle shooting
	Projectile* BossEnemyObject::Shoot(GLuint* textures) {
		// check if close enough to player (we are always chasing so wanna make sure that we arent shooting from 50 miles off the screen)
		// change to make sure player is in front as well (taken care of by checking that enemy is in pursuit/intercept)
		if (firing_timer_->Finished() && glm::length(target_->GetPosition() - position_) <= SHOOTING_RANGE && !exploding_ 
			&& (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
			firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

			// fire a basic projectile
			return new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, textures[tex_attacker_bullet], GetBearing(), player);
		}
		
		return nullptr;
	}
	void BossEnemyObject::Update(double delta_time, GLuint* textures)
	{
		GameObject::Update(delta_time, textures);
	}


BossPart::BossPart(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target, BossPart* parent)
	: EnemyGameObject(position, geom, shader, texture, target)
{
}
void BossPart::Update(double delta_time, GLuint* textures) {
	// Track the player
	glm::vec3 direction_to_player = glm::normalize(target_->GetPosition() - position_);
	angle_ = atan2(direction_to_player.y, direction_to_player.x);

	// Update local transform
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, 1.0f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0, 0, 1));
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);
	local_transform_ = translation * rotation * scaling;

	// If we have a parent, apply parent's world transform
	if (parent_) {
		world_transform_ = parent_->world_transform_ * local_transform_;
	}
	else {
		world_transform_ = local_transform_;
	}

	// Propagate to children
	for (auto* child : children_) {
		child->Update(delta_time, textures);
	}
}

Projectile* BossPart::Shoot(GLuint* textures)
{
	if (firing_timer_->Finished() && glm::length(target_->GetPosition() - position_) <= SHOOTING_RANGE && !exploding_
		&& (state_ == pursuit || state_ == intercepting) && !target_->GetExploding()) {
		firing_timer_->Start(ENEMY_FIRING_COOLDOWN);

		// fire a basic projectile
		return new Projectile(position_ + radius_ * GetBearing(), geometry_, shader_, textures[tex_attacker_bullet], GetBearing(), player);
	}

	return nullptr;
}
void BossPart::Render(glm::mat4 view_matrix, double current_time) {
	shader_->Enable();

	shader_->SetUniform1i("ghost", ghost_);
	shader_->SetUniform1f("tex_size", tex_size_);
	shader_->SetUniformMat4("view_matrix", view_matrix);

	shader_->SetUniformMat4("transformation_matrix", world_transform_);

	geometry_->SetGeometry(shader_->GetShaderProgram());
	glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);

	// Recursively draw children
	for (auto* child : children_) {
		child->Render(view_matrix, current_time);
	}
}

void BossPart::AddChild(BossPart* child) {
	children_.push_back(child);
	child->parent_ = this;
}
}