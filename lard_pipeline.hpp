#pragma once

#include "lard_device.hpp"

#include <string>
#include <vector>

namespace lard {

    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
        
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
};

    class LardPipeline {
        public:
            LardPipeline(LardDevice &device,
                const std::string &vertFilepath,
                const std::string &fragFilepath,
                const PipelineConfigInfo &configInfo);
            ~LardPipeline();
            LardPipeline(const LardPipeline&) = delete;
            LardPipeline& operator=(const LardPipeline&) = delete;
            void bind(VkCommandBuffer commandBuffer);
            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
        private:
            static std::vector<char> readFile(const std::string &filepath);
            void createGraphicsPipeline(const std::string &vertFilepath,
                const std::string &fragFilepath,
                const PipelineConfigInfo &configInfo);
            void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);
            LardDevice &lardDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertSaherModule;
            VkShaderModule fragShaderModule;
    };
}