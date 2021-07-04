#pragma once

#include "lard_model.hpp"

#include <memory>

namespace lard {
    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;
        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};
            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y }};
            return rotMatrix * scaleMat;
        }
    };

    class LardGameObject {
        public:
        using id_t = unsigned int;
        static LardGameObject createGameObject() {
            static id_t currentId = 0;
            return LardGameObject{currentId++};
        }
        LardGameObject(const LardGameObject &) = delete;
        LardGameObject &operator=(const LardGameObject &) = delete;
        LardGameObject(LardGameObject &&) = default;
        LardGameObject &operator=(LardGameObject &&) = default;
        const id_t getId() {
            return id;
        }
        std::shared_ptr<LardModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d;

        

        private:
        LardGameObject(id_t objId) : id{objId} {}
        id_t id;
    };
}