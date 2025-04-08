#ifndef EMP_BATTERY_COLLECTIBLE_H_
#define EMP_BATTERY_COLLECTIBLE_H_

#include "collectible_game_object.h"

namespace game {
	class EmpBatteryCollectible : public CollectibleGameObject {
		public:
			// Constructor
			EmpBatteryCollectible(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
	};
}

#endif