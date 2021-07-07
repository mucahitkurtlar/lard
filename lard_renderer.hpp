#pragma once

#include <memory>
#include <cassert>
#include <vector>

#include "lard_device.hpp"
#include "lard_swap_chain.hpp"
#include "lard_window.hpp"
#

namespace lard {
    class LardRenderer {
    public:
        LardRenderer(LardWindow& window, LardDevice& device);
        ~LardRenderer();
        LardRenderer(const LardRenderer&) = delete;
        LardRenderer& operator=(const LardRenderer&) = delete;

        bool isFrameInProgress() const {
            return isFrameStarted;
        }
        VkRenderPass getSwapChainRenderPass() const {
            return lardSwapChain->getRenderPass();
        }
        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameInProgress() && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentImageIndex];
        }
        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        LardWindow& lardWindow;
        LardDevice& lardDevice;
        std::unique_ptr<LardSwapChain> lardSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        bool isFrameStarted{ false };
    };
}