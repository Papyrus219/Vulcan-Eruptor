#ifndef ERUPTOR_HARDWARE_TEXTURE_HPP
#define ERUPTOR_HARDWARE_TEXTURE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_raii.hpp>

namespace eruptor::hardware
{

class Texture
{
public:

private:
    vma::raii::Image texture_image = nullptr;
};

}

#endif // ERUPTOR_HARDWARE_TEXTURE_HPP
