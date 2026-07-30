#include <Ovum/app.hpp>
#include <Eruptor/resource/colors.hpp>
#include <print>
#include <iostream>

using namespace ovum;

const std::filesystem::path ovum::App::scene_path_1{"../../scenes/test_1.papsc"};
const std::filesystem::path ovum::App::scene_path_2{"../../scenes/test_2.papsc"};
const std::filesystem::path ovum::App::scene_path_3{"../../scenes/test_3.papsc"};
const std::filesystem::path ovum::App::scene_path_4{"../../scenes/test_4.papsc"};
const std::filesystem::path ovum::App::scene_path_5{"../../scenes/test_5.papsc"};
const std::filesystem::path ovum::App::scene_path_6{"../../scenes/test_6.papsc"};
const std::filesystem::path ovum::App::scene_path_7{"../../scenes/test_7.papsc"};
const std::filesystem::path ovum::App::scene_path_8{"../../scenes/test_8.papsc"};
const std::filesystem::path ovum::App::scene_path_9{"../../scenes/test_9.papsc"};

const std::filesystem::path ovum::App::simulation_path_1{"../../simulations/test_1.papsim"};
const std::filesystem::path ovum::App::simulation_path_2{"../../simulations/test_2.papsim"};
const std::filesystem::path ovum::App::simulation_path_3{"../../simulations/test_3.papsim"};
const std::filesystem::path ovum::App::simulation_path_4{"../../simulations/test_4.papsim"};
const std::filesystem::path ovum::App::simulation_path_5{"../../simulations/test_5.papsim"};
const std::filesystem::path ovum::App::simulation_path_6{"../../simulations/test_6.papsim"};
const std::filesystem::path ovum::App::simulation_path_7{"../../simulations/test_7.papsim"};
const std::filesystem::path ovum::App::simulation_path_8{"../../simulations/test_8.papsim"};
const std::filesystem::path ovum::App::simulation_path_9{"../../simulations/test_9.papsim"};


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
    scene_saver.Assign_resource_manager(*resources);

    window = &renderer->Get_window();
    camera = &renderer->Get_camera();

    auto parsed_base_scene = scene_parser.Load_scene(current_scene_path);
    if(parsed_base_scene.has_value())
    {
        main_scene = parsed_base_scene.value();
    }
    else
    {
        std::print(std::cerr, "Error: {}\n", parsed_base_scene.error());
        std::exit(EXIT_FAILURE);
    }

    auto parsed_simulation_scene = simulation_parser.Load_simulation_data_into_scene(current_simulation_info_path, main_scene);
    if(parsed_simulation_scene.has_value())
    {
        main_scene = parsed_simulation_scene.value();
    }
    else
    {
        std::print(std::cerr, "Error: {}\n", parsed_simulation_scene.error());
        std::exit(EXIT_FAILURE);
    }

    for(auto & entity : main_scene.entieties)
    {
        entity.ai_state.curr_y_rot = glm::eulerAngles( main_scene.render_objects[ entity.render_object_id ].Get_rotaion() ).y;
        entity.ai_state.curr_y_rot = glm::eulerAngles( main_scene.render_objects[ entity.render_object_id ].Get_rotaion() ).y;
    }

    auto floor_it = main_scene.objects_aliases.find("floor");
    if(floor_it != main_scene.objects_aliases.end())
    {
        auto floor_aabb = main_scene.render_objects[ floor_it->second ].Get_aabb();

        world_min = floor_aabb.min;
        world_max = floor_aabb.max;

        std::println(std::clog, "Find world boundries: min: {} {} {} max: {} {} {}", world_min.x, world_min.y, world_min.z, world_max.x, world_max.y, world_max.z);
    }

    gp_comm.Enable_2d("Position");
    gp_comm.Set_x_axis_title("X coord");
    gp_comm.Set_y_axis_title("Z coord");
    gp_comm.Set_x_axis_range(world_min.x, world_max.x);
    gp_comm.Set_y_axis_range(world_min.z, world_max.z);

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
    physic_manager->Chceck_colisions( main_scene );

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

    Update_ai(delta_time.count());
    last_time = app_clock.now();
}

void ovum::App::Render()
{
    renderer->Stage_scene_render_data( main_scene );

    renderer->Flush_render_buffor();
}

void ovum::App::Update_ai(float delta_time)
{
    float rotation_speed{1.0f};

    for(auto & entity : main_scene.entieties)
    {
        auto & render_object = main_scene.render_objects[ entity.render_object_id ];

        if(entity.ai_state.is_desire_rot)
        {
            time_elapsed += delta_time;
            if(time_elapsed >= 5)
            {
                time_elapsed = 0;
                entity.ai_state.is_desire_rot = false;
                entity.ai_state.desire_y_rot += rotation_distributor(generator);
                entity.ai_state.desire_y_rot = glm::mod(entity.ai_state.desire_y_rot, glm::two_pi<float>());;
            }
        }
        else
        {
            float diff = std::abs( entity.ai_state.curr_y_rot - entity.ai_state.desire_y_rot );
            if(diff > 1.0f)
            {
                if(entity.ai_state.curr_y_rot < entity.ai_state.desire_y_rot)
                {
                    render_object.Rotate({0.0f, rotation_speed * delta_time, 0.0f});
                    entity.ai_state.curr_y_rot += rotation_speed * delta_time;
                }
                else
                {
                    render_object.Rotate({0.0f, -rotation_speed * delta_time, 0.0f});
                    entity.ai_state.curr_y_rot -= rotation_speed * delta_time;
                }
            }
            else
            {
                entity.ai_state.is_desire_rot = true;
            }
        }

        glm::vec3 forward  = render_object.Get_rotaion() * glm::vec3{1.0f, 0.0f, 0.0f} ;
        render_object.Move( forward * entity.speed );

        glm::vec3 pos = render_object.Get_position();
        bool hit_wall = false;

        if(pos.x > world_max.x) {pos.x = world_max.x; hit_wall = true;}
        else if(pos.x < world_min.x) {pos.x = world_min.x; hit_wall = true;}

        if(pos.z > world_max.z) {pos.z = world_max.z; hit_wall = true;}
        else if(pos.z < world_min.z) {pos.z = world_min.z; hit_wall = true;}

        if(hit_wall)
        {
            render_object.Set_position(pos);

            if(pos.x == world_max.x || pos.x == world_min.x) forward.x = -forward.x;
            if(pos.z == world_max.z || pos.z == world_min.z) forward.z = -forward.z;

            float new_y_rot = std::atan2(-forward.z, forward.x);

            entity.ai_state.curr_y_rot = new_y_rot;
            entity.ai_state.desire_y_rot = new_y_rot;
            entity.ai_state.is_desire_rot = false;

            render_object.Set_rotation_quad( glm::angleAxis(new_y_rot, glm::vec3{0.0f, 1.0f, 0.0f}) );
        }
    }
}

void ovum::App::Reload_scene()
{
    auto parsed_scene = scene_parser.Load_scene(current_scene_path);
    if(parsed_scene)
    {
        main_scene = *parsed_scene;
    }
    else
    {
        std::print(std::cerr, "Error: {}\n", parsed_scene.error());
    }

    auto parsed_simulation_scene = simulation_parser.Load_simulation_data_into_scene(current_simulation_info_path, main_scene);
    if(parsed_simulation_scene.has_value())
    {
        main_scene = parsed_simulation_scene.value();
    }
    else
    {
        std::println(std::cerr, "Error: {}", parsed_simulation_scene.error());
        std::exit(EXIT_FAILURE);
    }
}

void ovum::App::Save_scene()
{
    auto result = scene_saver.Save_scene_data(main_scene, current_scene_path);
    if(!result.has_value())
    {
        std::println(std::cerr, "Error: {}", result.error());
        std::exit(EXIT_FAILURE);
    }

    result = simulation_saver.Save_simulation_data(main_scene, current_simulation_info_path);
    if(!result.has_value())
    {
        std::println(std::cerr, "Error: {}", result.error());
        std::exit(EXIT_FAILURE);
    }
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

        //std::visit(hitbox_loger, main_scene.render_objects[ colision->object_a_id ].Get_hitbox());
        //std::visit(hitbox_loger, main_scene.render_objects[ colision->object_b_id ].Get_hitbox());
    }
    else if(auto key_pressed = event.Get_if<eruptor::event::Event::Key_pressed>())
    {
        if(key_pressed->key_type == eruptor::event::Key::R)
        {
            Reload_scene();
        }
        else if(key_pressed->key_type == eruptor::event::Key::TAB)
        {
            Save_scene();
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_1)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_1;
            }
            else
            {
                current_scene_path = scene_path_1;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_2)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_2;
            }
            else
            {
                current_scene_path = scene_path_2;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_3)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_3;
            }
            else
            {
                current_scene_path = scene_path_3;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_4)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_4;
            }
            else
            {
                current_scene_path = scene_path_4;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_5)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_5;
            }
            else
            {
                current_scene_path = scene_path_5;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::MUM_6)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_6;
            }
            else
            {
                current_scene_path = scene_path_6;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_7)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_7;
            }
            else
            {
                current_scene_path = scene_path_7;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_8)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_8;
            }
            else
            {
                current_scene_path = scene_path_8;
            }
        }
        else if(key_pressed->key_type == eruptor::event::Key::NUM_9)
        {
            if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
            {
                current_simulation_info_path = simulation_path_9;
            }
            else
            {
                current_scene_path = scene_path_9;
            }
        }

        std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
    }
}

void ovum::App::Hitbox_loger::operator()(const eruptor::physic::Sphere_hitbox & hitbox)
{
    std::print(std::clog, "Sphere -> center: {} {} {} radius: {}\n\n", hitbox.center.x, hitbox.center.y, hitbox.center.z, hitbox.radius);
}

void ovum::App::Hitbox_loger::operator()(const eruptor::physic::OBB_hitbox & hitbox)
{
    std::print(std::clog, "OBB -> center: {} {} {}\n", hitbox.center.x, hitbox.center.y, hitbox.center.z);
    std::print(std::clog, "axis_x: {} {} {} axis_y: {} {} {} axis_z: {} {} {} \n", hitbox.axies[0].x, hitbox.axies[0].y, hitbox.axies[0].z, hitbox.axies[1].x, hitbox.axies[1].y, hitbox.axies[1].z, hitbox.axies[2].x, hitbox.axies[2].y, hitbox.axies[2].z);
    std::print(std::clog, "half_width: {} {} {}\n\n", hitbox.half_width.x, hitbox.half_width.y, hitbox.half_width.z);
}

void ovum::App::Hitbox_loger::operator()(const eruptor::physic::Capsule_hitbox & hitbox)
{
    std::print(std::clog, "Capsule -> start: {} {} {}  end: {} {} {} radius: {}\n", hitbox.start.x, hitbox.start.y, hitbox.start.z, hitbox.end.x, hitbox.end.y, hitbox.end.z, hitbox.radius);
}
