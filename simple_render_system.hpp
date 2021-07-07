#pragma once

#include <memory>
#include <vector>

#include "lard_game_object.hpp"
#include "lard_pipeline.hpp"
#include "lard_device.hpp"

namespace lard {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(LardDevice& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();
        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LardGameObject>& gameObjects);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        LardDevice& lardDevice;
        std::unique_ptr<LardPipeline> lardPipeline;
        VkPipelineLayout pipelineLayout;
    };
}