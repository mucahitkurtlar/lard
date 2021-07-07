#include "lard_renderer.hpp"

#include <array>
#include <cassert>
#include <stdexcept>


namespace lard {

    LardRenderer::LardRenderer(LardWindow& window, LardDevice& device) : lardWindow{ window }, lardDevice{ device } {
        recreateSwapChain();
        createCommandBuffers();
    }

    LardRenderer::~LardRenderer() {
        freeCommandBuffers();
    }

    void LardRenderer::recreateSwapChain() {
        auto extent = lardWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = lardWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(lardDevice.device());

        if (lardSwapChain == nullptr) {
            lardSwapChain = std::make_unique<LardSwapChain>(lardDevice, extent);
        } else {
            std::shared_ptr<LardSwapChain> oldSwapChain = std::move(lardSwapChain);
            lardSwapChain = std::make_unique<LardSwapChain>(lardDevice, extent, oldSwapChain);
            if (oldSwapChain->compareSwapFormats(*lardSwapChain.get())) {
                throw std::runtime_error("Swap chain image or depth format has changed");
            }
            if (lardSwapChain->imageCount() != commandBuffers.size()) {
                freeCommandBuffers();
                createCommandBuffers();
            }

        }
    }

    void LardRenderer::createCommandBuffers() {
        commandBuffers.resize(lardSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = lardDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(lardDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }
    }

    void LardRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(lardDevice.device(), lardDevice.getCommandPool(), static_cast<float>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer LardRenderer::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");

        auto result = lardSwapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }
        isFrameStarted = true;
        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        return commandBuffer;
    }

    void LardRenderer::endFrame() {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }
        auto result = lardSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lardWindow.wasWindowResized()) {
            lardWindow.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image!");
        }

        isFrameStarted = false;
    }

    void LardRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on commanf buffer fromm a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = lardSwapChain->getRenderPass();
        renderPassInfo.framebuffer = lardSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = lardSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(lardSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(lardSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, lardSwapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void LardRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on commanf buffer fromm a different frame");


        vkCmdEndRenderPass(commandBuffer);
    }
}