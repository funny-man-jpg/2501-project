#include "hud.h"

namespace game {
	// Constructor/Destructor
	HUD::HUD(const glm::vec3& position, Geometry* geom, Shader* shader, Shader* text_shader, GLuint texture, GLuint health_tex, GLuint emp_tex, GLuint text_tex, int *score, PlayerGameObject* player)
		: GameObject(position, geom, shader, texture) {



		some_int = 0;

		// set up player pointer
		player_ = player;

		// set up score pointer
		score_ = score;

		// set up the health
		for (int i = 0; i < MAX_HEALTH; i++) {
			hearts_.push_back(new HealthCollectible(player_->GetPosition() + glm::vec3(X_HEALTH + X_HEALTH_GAP * i, Y_HEALTH, Z), geom, shader, health_tex));
		}

		// set up the emps
		for (int i = 0; i < MAX_EMP_BATTERY; i++) {
			emps_.push_back(new EmpBatteryCollectible(player_->GetPosition() + glm::vec3(X_EMP + X_EMP_GAP * i, Y_EMP, Z), geom, shader, emp_tex));
			emps_[i]->SetGhost(true);
		}

		// set up the score display
		score_display_ = new TextGameObject(player_->GetPosition() + glm::vec3(0.0f, Y_SCORE, Z), geom, text_shader, text_tex);
		score_display_->SetRotation(0.0f);
		score_display_->SetScale(glm::vec2(4.0f, 0.75f));
		//std::string int_to_string = (std::string)some_int;
		score_display_->SetText("Score:" + std::to_string(*score_));
	}

	HUD::~HUD() {
		// delete hearts
		for (int i = 0; i < hearts_.size(); i++) {
			delete hearts_[i];
		}

		// delete emp charges
		for (int i = 0; i < emps_.size(); i++) {
			delete emps_[i];
		}

		// delete score display
		delete score_display_;
	}



	void HUD::Update(double delta_time, GLuint* textures) {
		// update hearts
		for (int i = 0; i < hearts_.size(); i++) {
			hearts_[i]->SetPosition(player_->GetPosition() + glm::vec3(X_HEALTH + X_HEALTH_GAP * i, Y_HEALTH, Z));

			if (player_->GetHealth() < i + 1) {
				hearts_[i]->SetGhost(true);
			}
			else {
				hearts_[i]->SetGhost(false);
			}
		}

		// update emp charges
		for (int i = 0; i < emps_.size(); i++) {
			emps_[i]->SetPosition(player_->GetPosition() + glm::vec3(X_EMP + X_EMP_GAP * i, Y_EMP, Z));

			if (player_->GetEmpBatteryCollected() < i + 1) {
				emps_[i]->SetGhost(true);
			}
			else {
				emps_[i]->SetGhost(false);
			}
		}

		some_int++;

		// update score display
		score_display_->SetPosition(player_->GetPosition() + glm::vec3(0.0f, Y_SCORE, Z));
		score_display_->SetText("Score: " + std::to_string(*score_));
	}

	void HUD::Render(glm::mat4 view_matrix, double current_time) {
		// render hearts
		for (int i = 0; i < hearts_.size(); i++) {
			hearts_[i]->Render(view_matrix, current_time);
		}

		// render emp charges
		for (int i = 0; i < emps_.size(); i++) {
			emps_[i]->Render(view_matrix, current_time);
		}

		// render score display
		score_display_->Render(view_matrix, current_time);
	}
}