#ifndef ERUPTOR_CORE_HPP
#define ERUPTOR_CORE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <string_view>

namespace eruptor
{

class Core
{
public:
    void Init();

    void Open_window(std::string_view title, glm::uvec2 size);

    void Test();


    vk::raii::Context context{};
    vk::raii::Instance instance = nullptr;
    vk::raii::PhysicalDevice physical_device = nullptr;

private:
    void Setup_glfw();
    void Create_instance();
    void Pick_physical_device();

    std::vector<const char *> Get_required_instance_extensions();

    std::vector<GLFWwindow*> windows{};

    const std::vector<char const*> validation_layers = {"VK_LAYER_KHRONOS_validation"};

};

}

#endif // ERUPTOR_CORE_HPP
