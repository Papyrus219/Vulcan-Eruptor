#include <Eruptor/lib/hardware.hpp>
#include <Eruptor/lib/hardware/memory_units.hpp>

void eruptor::hardware::Hardware::Init()
{
    using namespace units;

    window.Init();
    window.Create_window( "Test", {800,600} );
    core.Init(window.Get_glfw_window());
    device.Init(core);
    swapchain.Init(device, window, core.Get_surface_handle());
    pipeline.Init(device, swapchain);
    command_manager.Init(device, MAX_FRAMES_IN_FLIGHT);
    geometry_buffer.Init( device.Get_alocator_handle(), 20_GiB, 10_GiB);
}
