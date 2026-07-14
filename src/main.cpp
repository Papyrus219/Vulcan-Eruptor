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

    auto handle_1 = men.Add_model("../../tmp_models/cube.obj");

    men.Load_models();

    while(rend.Is_window_open())
    {
        rend.Begin_frame();

        rend.Render_model( handle_1 );

        rend.End_frame();
    }

    return 0;
}
