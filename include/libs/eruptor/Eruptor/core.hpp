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
    vk::raii::Device device = nullptr;

private:
    void Setup_glfw();
    void Create_instance();
    void Pick_physical_device();
    void Create_logical_device();

    bool Is_device_sutiable(const vk::raii::PhysicalDevice & device);

    std::vector<const char *> Get_required_instance_extensions();

    uint32_t graphics_index{};
    uint32_t transfer_index{};
    uint32_t compute_index{};

    vk::raii::Queue graphics_queue = nullptr;
    vk::raii::Queue transfer_queue = nullptr;
    vk::raii::Queue compute_queue = nullptr;

    std::vector<GLFWwindow*> windows{};
    const std::vector<char const*> validation_layers = {"VK_LAYER_KHRONOS_validation"};

};

}

#endif // ERUPTOR_CORE_HPP
