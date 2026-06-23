#ifndef ERUPTOR_PLATFORM_PLATFORM_HPP
#define ERUPTOR_PLATFORM_PLATFORM_HPP

#include <Eruptor/lib/platform/core.hpp>
#include <Eruptor/lib/platform/device.hpp>
#include <Eruptor/lib/platform/window.hpp>
#include <Eruptor/lib/platform/swapchain.hpp>

namespace eruptor::platform
{

class Platform
{
public:
    void Init();

    Core & Get_core() {return core;}
    Window & Get_window() {return window;}
    Device & Get_device() {return device;}
    Swapchain & Get_swapchain() {return swapchain;}

private:
    Core core{};
    Device device{};
    Window window{};
    Swapchain swapchain{};
};

}

#endif // ERUPTOR_PLATFORM_PLATFORM_HPP
