#ifndef ERUPTOR_PLATFORM_PLATFORM_HPP
#define ERUPTOR_HARDWARE_HARDWARE_HPP

#include <Eruptor/lib/hardware/core.hpp>
#include <Eruptor/lib/hardware/device.hpp>
#include <Eruptor/lib/hardware/window.hpp>
#include <Eruptor/lib/hardware/swapchain.hpp>

namespace eruptor::hardware
{

class Hardware
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
