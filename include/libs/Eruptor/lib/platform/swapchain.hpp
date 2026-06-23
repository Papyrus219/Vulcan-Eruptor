#ifndef ERUPTOR_PLATFORM_SWAPCHAIN_HPP
#define ERUPTOR_PLATFORM_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace eruptor::platform
{

class Core;
class Device;
class Window;

class Swapchain
{
public:
    void Init(Device & device, Window & window, const vk::raii::SurfaceKHR & surface);

private:
    void Create_swap_chain(Device & device, Window & window, const vk::raii::SurfaceKHR & surface);
    void Create_image_views(Device & device);

    vk::SurfaceFormatKHR Choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR> & avalible_formats);
    vk::PresentModeKHR Choose_swap_present_mode(const std::vector<vk::PresentModeKHR> & avalible_present_modes);
    vk::Extent2D Choose_swap_extent(Window & window, const vk::SurfaceCapabilitiesKHR & capabilities);
    uint32_t Choose_swap_min_image_count(const vk::SurfaceCapabilitiesKHR & capabilities);

    vk::raii::SwapchainKHR swap_chain = nullptr;
    std::vector<vk::Image> swap_chain_images{};
    std::vector<vk::ImageView> swap_chain_image_views{};
    vk::SurfaceFormatKHR swap_chain_surface_format{};
    vk::Extent2D swap_chain_extent{};
};

}

#endif // ERUPTOR_PLATFORM_SWAPCHAIN_HPP
