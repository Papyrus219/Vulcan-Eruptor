#include <Eruptor/lib/hardware.hpp>

void eruptor::hardware::Hardware::Init()
{
    window.Init();
    window.Create_window( "Test", {800,600} );
    core.Init(window.Get_glfw_window());
    device.Init(core);
    swapchain.Init(device, window, core.Get_surface_handle());
    pipeline.Init(device, swapchain);
    command_manager.Init(device, MAX_FRAMES_IN_FLIGHT);
    vertex_buffer.Init( device.Get_alocator_handle() );
}
