#include "boss_enemy_game_object.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {
	// Constructor
	BossEnemyObject::BossEnemyObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target)
		: EnemyGameObject(position, geom, shader, texture, target) {
		float pi_over_two = glm::pi<float>() / 2.0f;
		base_part_ = new BossPart(position, geom, shader, texture, target, 1);
		gun_arm_ = new BossPart(glm::vec3(1, 0, 0), geom, shader, texture, target, 2);
		barrel_ = new BossPart(glm::vec3(1, 0, 0), geom, shader, texture, target, 3);
		base_part_->AddChild(gun_arm_);
		gun_arm_->AddChild(barrel_);
		radius_ = 6;

		score_value_ = 250;
	}

	// handle shooting
	std::vector<Projectile*>* BossEnemyObject::Shoot(GLuint* textures) {
		
		return barrel_->Shoot(textures);
	}
	void BossEnemyObject::Update(double delta_time, GLuint* textures)
	{
		base_part_->Update(delta_time, textures);
		//GameObject::Update(delta_time, textures);
	}

	void BossEnemyObject::Render(glm::mat4 view_matrix, double current_time)
	{
		base_part_->Render(view_matrix, current_time);
		//EnemyGameObject::Render(view_matrix, current_time);
	}


	BossPart::BossPart(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target, int type, BossPart* parent)
	: EnemyGameObject(position, geom, shader, texture, target)
	{
		parent_ = parent;
		partType = type;
	}

	// get forwards direction
	glm::vec3 BossPart::GetBearing(void) const {
		if (parent_ != nullptr) {
			return GameObject::GetBearing() + parent_->GetBearing();
		}
		else {
			return GameObject::GetBearing();
		}
	}

void BossPart::Update(double delta_time, GLuint* textures) {
	// Track the player
	glm::vec3 direction_to_player = glm::normalize(target_->GetPosition() - glm::vec3(world_transform_[3]));
	if (partType == 2) {
		//direction_to_player = glm::normalize(target_->GetPosition() - parent_->position_);
		//angle_ = atan2(direction_to_player.y, direction_to_player.x) - glm::half_pi<float>();
		glm::vec3 world_pos = glm::vec3(world_transform_[3]);
		glm::vec3 to_player = glm::normalize(target_->GetPosition() - world_pos);

		float full_angle = atan2(to_player.y, to_player.x);

		angle_ = 0.7f * full_angle;

	}
	else if (partType == 3) {
		glm::vec3 world_pos = glm::vec3(world_transform_[3]);
		glm::vec3 to_player = glm::normalize(target_->GetPosition() - world_pos);

		float full_angle = atan2(to_player.y, to_player.x);

		float parent_angle = atan2(parent_->world_transform_[0][1], parent_->world_transform_[0][0]);

		angle_ = full_angle - parent_angle;
	}
	else {
		scale_ = glm::vec2(5, 5);
	}

	// Update local transform
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, 1.0f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0, 0, 1));
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);

	// If we have a parent, apply parent's world transform
	if (parent_) {
		world_transform_ = parent_->world_transform_ * translation * rotation;
	}
	else {
		world_transform_ = translation * rotation;
	}
	local_transform_ = world_transform_ * scaling;

	// update children
	for (auto* child : children_) {
		child->Update(delta_time, textures);
	}
}

std::vector<Projectile*>* BossPart::Shoot(GLuint* textures) {
	std::vector<Projectile*>* vec = new std::vector<Projectile*>;

	// check if close enough to player (we are always chasing so wanna make sure that we arent shooting from 50 miles off the screen)
	// change to make sure player is in front as well (taken care of by checking that enemy is in pursuit/intercept)
	if (firing_timer_->Finished() && glm::length(target_->GetPosition() - glm::vec3(world_transform_[3])) <= SHOOTING_RANGE && !exploding_
		&& !target_->GetExploding()) {
		firing_timer_->Start(ENEMY_FIRING_COOLDOWN);
		//glm::vec3 dir = glm::vec3(glm::normalize(glm::vec2(world_transform_[0][0], world_transform_[1][0])), 0);


		// fire a basic projectile
		vec->push_back(new Projectile(glm::vec3(world_transform_[3]) + radius_ * GetBearing(), geometry_, shader_, textures[tex_attacker_bullet], GetBearing(), player));
	}

	return vec;
}
void BossPart::Render(glm::mat4 view_matrix, double current_time) {
	// Recursively draw children
	for (auto* child : children_) {
		child->Render(view_matrix, current_time);
	}

	
	shader_->Enable();

	shader_->SetUniform1i("ghost", ghost_);
	shader_->SetUniform1f("tex_size", tex_size_);
	shader_->SetUniformMat4("view_matrix", view_matrix);

	shader_->SetUniformMat4("transformation_matrix", local_transform_);

	geometry_->SetGeometry(shader_->GetShaderProgram());
	glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);

	
}

void BossPart::AddChild(BossPart* child) {
	children_.push_back(child);
	child->parent_ = this;
}
}