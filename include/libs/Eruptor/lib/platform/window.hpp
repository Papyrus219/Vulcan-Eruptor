#ifndef ERUPTOR_PLATFORM_WINDOW_HPP
#define ERUPTOR_PLATFORM_WINDOW_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string_view>

namespace eruptor::platform
{

class Core;

class Window
{
public:
    void Init();
    void Create_window(std::string_view title, glm::uvec2 size);

    GLFWwindow * Get_glfw_window() {return window;}

private:
    GLFWwindow * window{};

};

}

#endif // ERUPTOR_PLATFORM_WINDOW_HPP
