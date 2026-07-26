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
    physic_manager = &engine.Get_physic_manager();
    scene_parser.Assign_resource_manager(*resources);

    window = &renderer->Get_window();
    camera = &renderer->Get_camera();

    auto parsed_scene = scene_parser.Load_scene(main_scene_path);
    if(parsed_scene)
    {
        main_scene = *parsed_scene;
    }
    else
    {
        std::print(std::cerr, "Error: {}\n", parsed_scene.error());
        std::exit(EXIT_FAILURE);
    }

    auto & blob_1 = main_scene.render_objects[ main_scene.objects_aliases.at("blob_1") ];
    auto & blob_2 = main_scene.render_objects[ main_scene.objects_aliases.at("blob_2") ];
    auto & ball_1 = main_scene.render_objects[ main_scene.objects_aliases.at("ball_1") ];
    auto & ball_2 = main_scene.render_objects[ main_scene.objects_aliases.at("ball_2") ];

    blob_1.color = Color{174, 21, 23};
    blob_2.color = Color{20, 194, 34};
    ball_1.color = Color{13, 32, 199};
    ball_2.color = Color{13, 32, 199};

    gp_comm.Enable_2d("Position");
    gp_comm.Set_x_axis_title("X coord");
    gp_comm.Set_y_axis_title("Z coord");
    gp_comm.Set_x_axis_range(-10.0, 10.0);
    gp_comm.Set_y_axis_range(-10.0, 10.0);

    last_time = app_clock.now();
}

void ovum::App::Start_loop()
{
    while(is_running)
    {
        Update();

        gp_comm.Begin_frame();

        auto blob_aabb = main_scene.render_objects[2].Get_aabb();
        gp_comm.Stage_data({blob_aabb.min.x, blob_aabb.min.z});
        gp_comm.Stage_data({blob_aabb.max.x, blob_aabb.max.z});

        blob_aabb = main_scene.render_objects[3].Get_aabb();
        gp_comm.Stage_data({blob_aabb.min.x, blob_aabb.min.z});
        gp_comm.Stage_data({blob_aabb.max.x, blob_aabb.max.z});

        gp_comm.End_frame();

        Render();
    }
}

void ovum::App::Update()
{
    window->Update();
    physic_manager->Chceck_aabbs_colisions( main_scene );

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
        glm::vec3 dir = main_scene.render_objects[2].Get_rotaion() * glm::vec3(1.f, 0.f, 0.f);
        main_scene.render_objects[2].Move( dir * delta_time.count() );
    }
    if(window->Is_key_pressed(eruptor::event::Key::DOWN))
    {
        glm::vec3 dir = main_scene.render_objects[2].Get_rotaion() * glm::vec3(1.f, 0.f, 0.f);
        main_scene.render_objects[2].Move( -dir * delta_time.count() );
    }
    if(window->Is_key_pressed(eruptor::event::Key::LEFT))
    {
        main_scene.render_objects[2].Rotate(glm::vec3{0.0f, 1.0f, 0.0f} * delta_time.count());
    }
    if(window->Is_key_pressed(eruptor::event::Key::RIGHT))
    {
        main_scene.render_objects[2].Rotate(glm::vec3{0.0f, -1.0f, 0.0f} * delta_time.count());
    }
    if(window->Is_key_pressed(eruptor::event::Key::PLUS))
    {
        main_scene.render_objects[2].Change_scale( glm::vec3(2 * delta_time.count(), 2 * delta_time.count(), 2 * delta_time.count()), 0.1 );;
    }
    if(window->Is_key_pressed(eruptor::event::Key::MINUS))
    {
        main_scene.render_objects[2].Change_scale( glm::vec3(-2 * delta_time.count(), -2 * delta_time.count(), -2 * delta_time.count()), 0.1 );;
    }

    last_time = app_clock.now();
}

void ovum::App::Render()
{
    renderer->Stage_scene_render_data( main_scene );

    renderer->Flush_render_buffor();
}

void ovum::App::Reload_scene()
{
    auto parsed_scene = scene_parser.Load_scene(main_scene_path);
    if(parsed_scene)
    {
        main_scene = *parsed_scene;
    }
    else
    {
        std::print(std::cerr, "Error: {}\n", parsed_scene.error());
        std::exit(EXIT_FAILURE);
    }

    auto & blob_1 = main_scene.render_objects[ main_scene.objects_aliases.at("blob_1") ];
    auto & blob_2 = main_scene.render_objects[ main_scene.objects_aliases.at("blob_2") ];
    auto & ball_1 = main_scene.render_objects[ main_scene.objects_aliases.at("ball_1") ];
    auto & ball_2 = main_scene.render_objects[ main_scene.objects_aliases.at("ball_2") ];

    blob_1.color = Color{174, 21, 23};
    blob_2.color = Color{20, 194, 34};
    ball_1.color = Color{13, 32, 199};
    ball_2.color = Color{13, 32, 199};
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
    else if(auto colision = event.Get_if<eruptor::event::Event::Collision_occurred>())
    {
        //std::print(std::clog, "Collision occurred: Object a: {} Object b: {}\n", colision->object_a_id, colision->object_b_id);
    }
    else if(auto key_pressed = event.Get_if<eruptor::event::Event::Key_pressed>())
    {
        if(key_pressed->key_type == eruptor::event::Key::R)
        {
            Reload_scene();
        }
    }
}

