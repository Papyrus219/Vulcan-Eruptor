#include <Eruptor/lib/platform.hpp>

void eruptor::platform::Platform::Init()
{
    window.Init();
    window.Create_window( "Test", {800,600} );
    core.Init(window.Get_glfw_window());
    device.Init(core);
    swapchain.Init(device, window, core.Get_surface_handle());
}
