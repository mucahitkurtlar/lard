#pragma once

#include <memory>
#include <vector>

#include "lard_window.hpp"
#include "lard_game_object.hpp"
#include "lard_pipeline.hpp"
#include "lard_device.hpp"
#include "lard_swap_chain.hpp"

namespace lard {
    class FirstApp {
        public:
            FirstApp();
            ~FirstApp();
            FirstApp(const FirstApp &) = delete;
            FirstApp &operator=(const FirstApp &) = delete;
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            void run();
        private:
            void loadGameObjects();
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame();
            void sierpinski(std::vector<LardModel::Vertex> &verticies, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);
            void recreateSwapChain();
            void recordCommandBuffer(int imageIndex);
            void renderGameObjects(VkCommandBuffer commandBuffer);

            LardWindow lardWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
            LardDevice lardDevice{lardWindow};
            std::unique_ptr<LardSwapChain> lardSwapChain;
            std::unique_ptr<LardPipeline> lardPipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::vector<LardGameObject> gameObjects;
    };
}