#ifndef HUD_H_
#define HUD_H_

#include <vector>

#include "game_object.h"
#include "health_collectible.h"
#include "emp_battery_collectible.h"
#include "player_game_object.h"
#include "text_game_object.h"

// z value to render everything on top
#define Z -2.0f

// offsets for health
#define X_HEALTH -4.5f
#define Y_HEALTH 3.5f
#define X_HEALTH_GAP 0.75f

// offsets for emp
#define X_EMP 4.5f
#define Y_EMP 3.5f
#define X_EMP_GAP -0.75f

// offsets for score display
#define Y_SCORE 3.5f

namespace game {
	class HUD : public GameObject {
		public:
			// Constructor/Destructor
			HUD(const glm::vec3& position, Geometry* geom, Shader* shader, Shader* text_shader, GLuint texture, GLuint health_tex, GLuint emp_tex, GLuint text_tex, int *score, PlayerGameObject *player);
			virtual ~HUD(void);

			virtual void Update(double delta_time, GLuint* textures);
			virtual void Render(glm::mat4 view_matrix, double current_time);

		private:
			// health display
			std::vector<HealthCollectible*> hearts_;

			// emp battery charges display
			std::vector<EmpBatteryCollectible*> emps_;

			// score display
			TextGameObject *score_display_;

			// pointer to the player
			PlayerGameObject *player_;

			// pointer to the score
			int *score_;

			int some_int;
	};
}

#endif