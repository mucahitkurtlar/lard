#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace lard {
    class LardWindow {
        public:
            LardWindow(int w, int h, std::string name);
            ~LardWindow();
            LardWindow(const LardWindow &) = delete;
            LardWindow &operator=(const LardWindow &) = delete;
            bool shouldClose();
            bool wasWindowResized();
            void resetWindowResizedFlag();
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
            VkExtent2D getExtent();

        private:
            void initWindow();
            static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

            int width;
            int height;
            bool framebufferResized = false;
            std::string windowName;
            GLFWwindow *window;
    };
}