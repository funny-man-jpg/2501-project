#include "projectile.h"

namespace game {
	Projectile::Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec3 direction, int target_type) 
		: GameObject(position, geom, shader, texture) {
		
		scale_ = glm::vec2(0.5f, 0.25f);
		type_ = projectile;
		collision_type_ = ray;

		// set up time trackers for the projectile
		curr_time_ = 0;
		prev_time_ = curr_time_;

		// set up the movement variables
		speed_ = 6;
		velocity_ = speed_ * direction;

		// give the projectile a lifespan
		timer_->Start(LIFESPAN);

		// set up the target trackers
		enemy_ = nullptr;
		impact_time_ = 0;
		soonest_impact_time_ = LIFESPAN * 2;

		// set up the target type
		target_type_ = target_type;

		SetRotation(glm::atan(direction.y, direction.x));
	}

	// Update function for moving the projectile
	void Projectile::Update(double delta_time, GLuint* textures) {
		prev_time_ = curr_time_;
		curr_time_ += delta_time;

		if (!exploding_) {
			float new_x = position_.x + velocity_.x * delta_time;
			float new_y = position_.y + velocity_.y * delta_time;
			position_ = glm::vec3(new_x, new_y, 0.0);
		}
		else {
			if (collideable_) {
				enemy_->Hit(textures, this);
				SetCollideability(false);
			}
		}

		if (timer_->Finished()) {
			delete_ = true;
		}
	}

	void Projectile::CheckForCollision(GLuint *textures, GameObject *other) {
		// do ray circle collision
		if (other->GetType() == target_type_) {
			// calculate the intersection times
			glm::vec3 d = this->GetBearing();
			glm::vec3 circlePartToRayPart = this->position_ - other->GetPosition();

			float a = glm::dot(d, d);
			float b = 2 * glm::dot(d, circlePartToRayPart);
			float c = glm::dot(circlePartToRayPart, circlePartToRayPart) - std::pow(other->GetRadius(), 2);

			float underTheSqrt = std::pow(b, 2) - (4 * a * c);

			// check if there is a possible collision
			if (underTheSqrt < 0) {
				return;
			}

			float laterTime = (-b + std::sqrt(underTheSqrt)) / (2 * a);
			float soonerTime = (-b - std::sqrt(underTheSqrt)) / (2 * a);

			// determine if a collision happened
			if ((prev_time_ > soonerTime && prev_time_ < laterTime) || (curr_time_ > soonerTime && curr_time_ < laterTime) || (prev_time_ < soonerTime && laterTime < curr_time_)) {
				impact_time_ = soonerTime;
				this->Hit(textures, other);
			}
		}
	}

	void Projectile::Hit(GLuint* textures, GameObject* other) {
		if (other->GetType() == target_type_) {
			texture_ = textures[tex_explosion];
			exploding_ = true;
			scale_ = glm::vec2(0.75f);
			timer_->Start(EXPLOSION_LENGTH);

			// keep track of who is the closest collision
			if (impact_time_ < soonest_impact_time_) {
				soonest_impact_time_ = impact_time_;
				enemy_ = other;
			}
		}
	}
}