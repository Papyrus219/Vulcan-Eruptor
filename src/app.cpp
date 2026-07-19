#include <app.hpp>
#include <colors.hpp>

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

    auto blob_model = resources->Add_model("../../tmp_models/blob/blob.fbx");

    resources->Load_models();

    main_scene.render_objects.push_back({});
    main_scene.render_objects.back().model_handle = platform_model;

    main_scene.render_objects.push_back({});
    main_scene.render_objects.back().model_handle = blob_model;
    main_scene.render_objects.back().parent_object_index = 1;
    main_scene.render_objects.back().transformation.Set_position({2.0f, 0.58f, 1.0f});
    main_scene.render_objects.back().transformation.Set_scale({0.01f, 0.01f, 0.01f});
    main_scene.render_objects.back().transformation.Set_rotation_euler({0.0f, 90.5f, 0.0f});
    main_scene.render_objects.back().color = Color{171, 24, 24};

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
        main_scene.render_objects[2].transformation.Set_position( main_scene.render_objects[2].transformation.Get_position() + glm::vec3{10 * delta_time.count(), 0, 0} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::DOWN))
    {
        main_scene.render_objects[2].transformation.Set_position( main_scene.render_objects[2].transformation.Get_position() + glm::vec3{-10 * delta_time.count(), 0, 0} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::LEFT))
    {
        main_scene.render_objects[2].transformation.Set_position( main_scene.render_objects[2].transformation.Get_position() + glm::vec3{0, 0, -10 * delta_time.count()} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::RIGHT))
    {
        main_scene.render_objects[2].transformation.Set_position( main_scene.render_objects[2].transformation.Get_position() + glm::vec3{0, 0, 10 * delta_time.count()} );
    }
    if(window->Is_key_pressed(eruptor::event::Key::PLUS))
    {
        main_scene.render_objects[2].transformation.Set_scale( main_scene.render_objects[2].transformation.Get_scale() + glm::vec3(2 * delta_time.count(), 2 * delta_time.count(), 2 * delta_time.count()) );;
    }
    if(window->Is_key_pressed(eruptor::event::Key::MINUS))
    {
        main_scene.render_objects[2].transformation.Set_scale( main_scene.render_objects[2].transformation.Get_scale() + glm::vec3(-2 * delta_time.count(), -2 * delta_time.count(), -2 * delta_time.count()) );;
    }

    last_time = app_clock.now();
}

void ovum::App::Render()
{
    renderer->Begin_frame();

    renderer->Render_scene( main_scene );

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

