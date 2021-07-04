#include <stdexcept>

#include "lard_window.hpp"

namespace lard {
    LardWindow::LardWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
        initWindow();
    }

    LardWindow::~LardWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool LardWindow::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    bool LardWindow::wasWindowResized() {
        return framebufferResized;
    }

    void LardWindow::resetWindowResizedFlag() {
        framebufferResized = false;
    }

    void LardWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
    }

    VkExtent2D LardWindow::getExtent() {
        return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    void LardWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void LardWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto lardWindow = reinterpret_cast<LardWindow *>(glfwGetWindowUserPointer(window));
        lardWindow->framebufferResized = true;
        lardWindow->width = width;
        lardWindow->height = height;
    }
}
