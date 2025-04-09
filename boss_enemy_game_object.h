#ifndef BOSS_ENEMY_GAME_OBJECT_H_
#define BOSS_ENEMY_GAME_OBJECT_H_

#include "enemy_game_object.h"
#include<vector>

#define SHOOTING_RANGE 5.0

namespace game {

	class BossPart : public EnemyGameObject {
	public:
		BossPart(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target, BossPart* parent = nullptr);

		glm::mat4 local_transform_;
		glm::mat4 world_transform_;
		BossPart* parent_;
		std::vector<BossPart*> children_;
		void Update(double delta_time, GLuint* textures) override;
		void Render(glm::mat4 view_matrix, double current_time) override;
		virtual std::vector<Projectile*>* Shoot(GLuint* textures);
		void AddChild(BossPart* child);
	};


	class BossEnemyObject : public EnemyGameObject {
		public:
			// Constructor
			BossEnemyObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* target);

			// handle shooting
			virtual std::vector<Projectile*>* Shoot(GLuint* textures);
			void Update(double delta_time, GLuint* textures) override;
			void Render(glm::mat4 view_matrix, double current_time) override;
		private:
			BossPart* base_part_;
			BossPart* gun_arm_;
			BossPart* barrel_;
	};


	

}

#endif
