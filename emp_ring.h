#ifndef EMP_RING_H_
#define EMP_RING_H_

#include "game_object.h"

#define EMP_RING_DURATION 7
#define MAX_RING_SIZE 2

namespace game {
	class EmpRing : public GameObject {
		public:
			// Constructor/Destructor
			EmpRing(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent);

			// Update method
			virtual void Update(double delta_time, GLuint* textures);

		private:
			// pointer to the parent
			GameObject *parent_;
	};
}

#endif