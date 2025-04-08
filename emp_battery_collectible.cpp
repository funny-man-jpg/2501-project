#include "emp_battery_collectible.h"

namespace game {
	// Constructor
	EmpBatteryCollectible::EmpBatteryCollectible(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: CollectibleGameObject(position, geom, shader, texture) {

		type_ = emp_battery_collectible;
		scale_ = glm::vec2(0.25f, 0.5f);
		radius_ = 0.3f;
	}
}