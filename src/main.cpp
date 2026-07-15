#include <iostream>
#include <vulkan/vulkan.h>
#include <Eruptor/eruptor.hpp>
#include <Eruptor/lib/hardware/window.hpp>
#include <Eruptor/lib/event/event_listener.hpp>
#include <Eruptor/lib/event/event_manager.hpp>
#include <iostream>
#include <chrono>

bool window_is_open{true};
eruptor::renderer::Fly_camera * camera;

class Event_liss: public eruptor::event::Event_listener
{
public:
    Event_liss();
    void On_event(eruptor::event::Event & event) override;

private:
    eruptor::event::Event_manager & event_manager;
} event_liss;

Event_liss::Event_liss(): event_manager{ eruptor::event::event_manager }
{
    event_manager.Add_listener( *this );
}

void Event_liss::On_event(eruptor::event::Event & event)
{
    if(event.type == eruptor::event::Event_type::CLOSE_WINDOW)
    {
        window_is_open = false;
    }
    else if(event.type == eruptor::event::Event_type::MOUSE_MOVED)
    {
        camera->Process_mouse_movement(event.mouse_move_x_offset, event.mouse_move_y_offset);
    }
}

int main()
{
    eruptor::Eruptor test{};
    test.Init();

    auto & rend = test.Get_renderer();
    auto & men = test.Get_resource_manager();

    auto & window = rend.Get_window();
    camera = &rend.Get_camera();

    auto handle_1 = men.Add_model("../../tmp_models/cube.obj");

    men.Load_models();

    std::chrono::high_resolution_clock app_clock{};
    auto last_time = app_clock.now();

    while(window_is_open)
    {
        auto current_time = app_clock.now();
        std::chrono::duration<float> delta_time = current_time - last_time;
        last_time = current_time;

        if(window.Is_key_pressed(eruptor::event::Key::W))
        {
            camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::FORWARD, delta_time.count());
            std::clog << "Forward!\n";
        }
        if(window.Is_key_pressed(eruptor::event::Key::S))
        {
            camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::BACKWARD, delta_time.count());
        }
        if(window.Is_key_pressed(eruptor::event::Key::A))
        {
            camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::LEFT, delta_time.count());
        }
        if(window.Is_key_pressed(eruptor::event::Key::D))
        {
            camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::RIGHT, delta_time.count());
        }

        rend.Begin_frame();

        rend.Render_model( handle_1 );

        rend.End_frame();
    }

    return 0;
}
