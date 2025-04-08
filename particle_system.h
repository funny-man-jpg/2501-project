#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.h"
#include<iostream>

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent, bool expl);

            void Update(double delta_time, GLuint* textures) override;

            void Render(glm::mat4 view_matrix, double current_time);

        private:
            GameObject *parent_;
            bool explosion;
            std::string test;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
