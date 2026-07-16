#include <app.hpp>

using namespace ovum;


ovum::App::App() : event_manager{ eruptor::event::event_manager }
{
    event_manager.Add_listener(*this);
}

void ovum::App::Init()
{
    engine.Init();

    renderer = &engine.Get_renderer();
    resources = &engine.Get_resource_manager();

    window = &renderer->Get_window();
    camera = &renderer->Get_camera();

    auto handle_1 = resources->Add_model("../../tmp_models/cube.obj");
    models_handles.push_back(handle_1);

    resources->Load_models();

    last_time = app_clock.now();
}

void ovum::App::Start_loop()
{
    while(is_running)
    {
        Update();

        Render();
    }
}

void ovum::App::Update()
{
    std::chrono::duration<float> delta_time = app_clock.now() - last_time;

    if(window->Is_key_pressed(eruptor::event::Key::W))
    {
        camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::FORWARD, delta_time.count());
    }
    if(window->Is_key_pressed(eruptor::event::Key::S))
    {
        camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::BACKWARD, delta_time.count());
    }
    if(window->Is_key_pressed(eruptor::event::Key::A))
    {
        camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::LEFT, delta_time.count());
    }
    if(window->Is_key_pressed(eruptor::event::Key::D))
    {
        camera->Process_keyboard(eruptor::renderer::Camera_movement_direction::RIGHT, delta_time.count());
    }

    last_time = app_clock.now();
}

void ovum::App::Render()
{
    renderer->Begin_frame();

    for(auto handle : models_handles)
    {
        renderer->Render_model(handle);
    }

    renderer->End_frame();
}

void ovum::App::On_event(const eruptor::event::Event& event)
{
    if(event.Is<eruptor::event::Event::Close_window>())
    {
        is_running = false;
    }
    else if(auto mouse_move = event.Get_if<eruptor::event::Event::Mouse_moved>())
    {
        camera->Process_mouse_movement(mouse_move->x_offset, mouse_move->y_offset);
    }
}

