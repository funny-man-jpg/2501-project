#include "emp_battery_collectible.h"

namespace game {
	// Constructor
	EmpBatteryCollectible::EmpBatteryCollectible(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: CollectibleGameObject(position, geom, shader, texture) {
	}
}