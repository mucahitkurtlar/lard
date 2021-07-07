#include "first_app.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>


namespace lard {
    FirstApp::FirstApp() {
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{ lardDevice, lardRenderer.getSwapChainRenderPass() };
        while (!lardWindow.shouldClose()) {
            glfwPollEvents();
            if (auto commandBuffer = lardRenderer.beginFrame()) {
                lardRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                lardRenderer.endSwapChainRenderPass(commandBuffer);
                lardRenderer.endFrame();
            }

        }

        vkDeviceWaitIdle(lardDevice.device());

    }

    void FirstApp::loadGameObjects() {


        std::vector<LardModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        //std::vector<LardModel::Vertex> vertices{};
        //sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

        auto lardModel = std::make_shared<LardModel>(lardDevice, vertices);

        auto triangle = LardGameObject::createGameObject();
        triangle.model = lardModel;
        triangle.color = { .1f, .8f, .1f };
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = { 2.f, .5f };
        triangle.transform2d.rotation = .25f * glm::two_pi<float>(); // radian

        gameObjects.push_back(std::move(triangle));

        /*
         std::vector<LardModel::Vertex> vertices{
             {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
             {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
             {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };
         auto lardModel = std::make_shared<LardModel>(lardDevice, vertices);

         std::vector<glm::vec3> colors{
             {1.f, .7f, .73f},
             {1.f, .87f, .73f},
             {1.f, 1.f, .73f},
             {.73f, 1.f, .8f},
             {.73, .88f, 1.f}  //
         };
         for (auto& color : colors) {
             color = glm::pow(color, glm::vec3{ 2.2f });
         }
         for (int i = 0; i < 40; i++) {
             auto triangle = LardGameObject::createGameObject();
             triangle.model = lardModel;
             triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
             triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
             triangle.color = colors[i % colors.size()];
             gameObjects.push_back(std::move(triangle));
         }
         */
    }
}