#ifndef ERUPTOR_HARDWARE_TEXTURE_HPP
#define ERUPTOR_HARDWARE_TEXTURE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_raii.hpp>

namespace eruptor::hardware
{

class Device;

class Texture
{
public:
    void Init(Device & device, int width, int height, vk::DeviceSize offset_in_stage_buffer);

private:
    vk::DeviceSize offset_in_stage_buffer{};

    vma::raii::Image texture_image = nullptr;
    vk::raii::ImageView texture_view = nullptr;
};

}

#endif // ERUPTOR_HARDWARE_TEXTURE_HPP
