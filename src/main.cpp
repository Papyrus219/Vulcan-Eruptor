#include <iostream>
#include <vulkan/vulkan.h>
#include <Eruptor/eruptor.hpp>
#include <iostream>

int main()
{
    eruptor::Eruptor test{};

    test.Init();

    auto & rend = test.Get_renderer();
    auto & men = test.Get_resource_manager();

    auto handle = men.Add_model("../../tmp_models/cube.obj");

    men.Load_models();

    while(rend.Is_window_open())
    {
        rend.Render_model( handle );
    }

    return 0;
}
