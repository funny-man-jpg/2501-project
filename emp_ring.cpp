#include "emp_ring.h"

namespace game {
	// Constructor/Destructor
	EmpRing::EmpRing(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject *parent)
		: GameObject(position, geom, shader, texture) {

		parent_ = parent;
		type_ = emp_ring;
		timer_->Start(EMP_RING_DURATION);
	}

	// Update method
	void EmpRing::Update(double delta_time, GLuint* textures) {
		if (timer_->Finished()) {
			collideable_ = false;
			delete_ = true;
		}
		else {
			// check if size has reached max
			if (scale_.x < MAX_RING_SIZE) {
				scale_ += glm::vec2(MAX_RING_SIZE * delta_time);

				// increase radius as well
				radius_ = scale_.x / 2.0f;
			}

			position_ = parent_->GetPosition();
		}
	}
}