#ifndef ERUPTOR_CORE_HPP
#define ERUPTOR_CORE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <string_view>

namespace eruptor::platform
{

class Core
{
public:
    void Init();

    auto Get_physical_devices() {return instance.enumeratePhysicalDevices();}

private:
    void Create_instance();

    std::vector<const char *> Get_required_instance_extensions();

    vk::raii::Context context{};
    vk::raii::Instance instance = nullptr;

    const std::vector<char const*> validation_layers = {"VK_LAYER_KHRONOS_validation"};
};

}

#endif // ERUPTOR_CORE_HPP
