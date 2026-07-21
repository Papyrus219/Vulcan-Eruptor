#include <app.hpp>
#include <colors.hpp>
#include <print>
#include <iostream>

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

    auto platform_model = resources->Add_model("../../tmp_models/platform/platform.obj");
    models_handles.push_back(platform_model);

    auto blob_model = resources->Add_model("../../tmp_models/blob/blob.obj");

    resources->Load_models();

    main_scene.render_objects.emplace_back(*resources, platform_model);
    main_scene.render_objects.emplace_back(*resources, platform_model);

    main_scene.render_objects.emplace_back(*resources, blob_model);
    main_scene.render_objects.back().color = Color{171, 24, 24};
    main_scene.render_objects.back().Set_scale( {0.2, 0.2, 0.2}, 0 );

    std::print(std::clog, "Objects amomont: {}\n", main_scene.render_objects.size());

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
    if(window->Is_key_pressed(eruptor::event::Key::UP))
    {
        main_scene.render_objects[2].Move( glm::vec3{10 * delta_time.count(), 0, 0} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::DOWN))
    {
        main_scene.render_objects[2].Move( glm::vec3{-10 * delta_time.count(), 0, 0} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::LEFT))
    {
        main_scene.render_objects[2].Move( glm::vec3{0, 0, -10 * delta_time.count()} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::RIGHT))
    {
        main_scene.render_objects[2].Move( glm::vec3{0, 0, 10 * delta_time.count()} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::PLUS))
    {
        main_scene.render_objects[2].Change_scale( glm::vec3(2 * delta_time.count(), 2 * delta_time.count(), 2 * delta_time.count()), 0 );;
    }
    if(window->Is_key_pressed(eruptor::event::Key::MINUS))
    {
        main_scene.render_objects[2].Change_scale( glm::vec3(-2 * delta_time.count(), -2 * delta_time.count(), -2 * delta_time.count()), 0 );;
    }

    last_time = app_clock.now();
}

void ovum::App::Render()
{
    renderer->Begin_frame();

    renderer->Render_scene( main_scene );

    auto blob_aabb = main_scene.render_objects[2].Get_aabb();
    std::print(std::clog, "Blob AABB.min: {}, {}, {}\n", blob_aabb.min.x, blob_aabb.min.y, blob_aabb.min.z);

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

