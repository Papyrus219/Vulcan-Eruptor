#ifndef ERUPTOR_PLATFORM_DEVICE_HPP
#define ERUPTOR_PLATFORM_DEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace eruptor::platform
{

class Core;

class Device
{
public:
    void Init(Core & core);

private:
    vk::raii::PhysicalDevice physical_device = nullptr;
    vk::raii::Device device = nullptr;

    void Pick_physical_device(Core & core);
    void Create_logical_device(Core & core);

    bool Is_device_sutiable(const vk::raii::PhysicalDevice & device);

    uint32_t graphics_index{};
    uint32_t transfer_index{};
    uint32_t compute_index{};

    vk::raii::Queue graphics_queue = nullptr;
    vk::raii::Queue transfer_queue = nullptr;
    vk::raii::Queue compute_queue = nullptr;
};

}

#endif // ERUPTOR_PLATFORM_DEVICE_HPP
