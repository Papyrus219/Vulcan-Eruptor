#ifndef ERUPTOR_HARDWARE_HARDWARE_HPP
#define ERUPTOR_HARDWARE_HARDWARE_HPP

#include <Eruptor/lib/hardware/core.hpp>
#include <Eruptor/lib/hardware/device.hpp>
#include <Eruptor/lib/hardware/window.hpp>
#include <Eruptor/lib/hardware/swapchain.hpp>
#include <Eruptor/lib/hardware/pipeline.hpp>
#include <Eruptor/lib/hardware/command_manager.hpp>
#include <Eruptor/lib/hardware/vertex_buffer.hpp>

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
    Pipeline & Get_pipeline() {return pipeline;}
    Command_manager & Get_command_manager() {return command_manager;}
    Vertex_buffer & Get_vertex_buffer() {return vertex_buffer;}

private:
    Core core{};
    Device device{};
    Window window{};
    Swapchain swapchain{};
    Pipeline pipeline{};
    Command_manager command_manager{};
    Vertex_buffer vertex_buffer{};

    static constexpr int MAX_FRAMES_IN_FLIGHT{2};
};

}

#endif // ERUPTOR_HARDWARE_HARDWARE_HPP
