#pragma once

#include <memory>
#include <vector>

#include "lard_window.hpp"
#include "lard_game_object.hpp"
#include "lard_pipeline.hpp"
#include "lard_device.hpp"
#include "lard_renderer.hpp"

namespace lard {
    class FirstApp {
    public:
        FirstApp();
        ~FirstApp();
        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
    private:
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeline();
        void renderGameObjects(VkCommandBuffer commandBuffer);

        LardWindow lardWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
        LardDevice lardDevice{ lardWindow };
        LardRenderer lardRenderer{ lardWindow, lardDevice };
        std::unique_ptr<LardPipeline> lardPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<LardGameObject> gameObjects;
    };
}