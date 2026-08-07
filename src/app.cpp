#include <Ovum/app.hpp>
#include <Eruptor/resource/colors.hpp>
#include <print>
#include <iostream>
#include <format>

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

    main_font = resources->Add_font_atlas("../../fonts/arial.ttf", 30);
    small_font = resources->Add_font_atlas("../../fonts/arial.ttf", 5);
    resources->Load_font_atlases();

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

    if(mode == Mode::SIMULATION)
    {
        Update_ai(delta_time.count());
    }

    last_time = app_clock.now();
}

void ovum::App::Render()
{
    renderer->Stage_scene_render_data( main_scene );

    renderer->Stage_text_render_data( std::format("Mode: {}", Get_string_from_mode_enum(mode)), 10, 40, main_font, {155, 20, 30, 255});

    if(mode == Mode::EDITOR)
    {
        renderer->Stage_text_render_data( std::format("Selected object type: {}", Get_string_from_object_type_enum( object_type )), 10, 80, main_font, {20, 40, 155, 255});

        renderer->Stage_text_render_data("M - Change mode", 480, 500, small_font, {255, 255, 255, 255});
        renderer->Stage_text_render_data("T - Change object type", 482, 530, small_font, {255, 255, 255, 255});
        renderer->Stage_text_render_data("N - Add new object", 482, 560, small_font, {255, 255, 255, 255});
    }

    renderer->Flush_render_buffor();
}

void ovum::App::Update_ai(float delta_time)
{
    float rotation_speed{1.0f};
    float wall_margin{3.0f};

    for(auto & entity : main_scene.entieties)
    {
        auto & render_object = main_scene.render_objects[ entity.render_object_id ];
        glm::vec3 pos = render_object.Get_position();

        bool near_wall{};
        glm::vec3 desired_dir = render_object.Get_rotaion() * glm::vec3{1.0f, 0.0f, 0.0f};

        if(pos.x > world_max.x - wall_margin)
        {
            desired_dir.x = -std::abs(desired_dir.x);
            near_wall = true;
        }
        else if(pos.x < world_min.x + wall_margin)
        {
            desired_dir.x = std::abs(desired_dir.x);
            near_wall = true;
        }

        if(pos.z > world_max.z - wall_margin)
        {
            desired_dir.z = -std::abs(desired_dir.z);
            near_wall = true;
        }
        else if(pos.z < world_min.z + wall_margin)
        {
            desired_dir.z = std::abs(desired_dir.z);
            near_wall = true;
        }

        if(near_wall)
        {
            entity.ai_state.desire_y_rot = std::atan2(-desired_dir.z, desired_dir.x);
            entity.ai_state.is_desire_rot = false;
        }

        if(entity.ai_state.is_desire_rot)
        {
            entity.ai_state.time_elapsed += delta_time;

            if(entity.ai_state.time_elapsed >= 5)
            {
                entity.ai_state.time_elapsed = 0;
                entity.ai_state.is_desire_rot = false;
                entity.ai_state.desire_y_rot += rotation_distributor(generator);
                entity.ai_state.desire_y_rot = glm::mod(entity.ai_state.desire_y_rot, glm::two_pi<float>());;
            }
        }
        else
        {
            float diff = std::abs( entity.ai_state.curr_y_rot - entity.ai_state.desire_y_rot );
            if(diff > 0.05f)
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

        pos = render_object.Get_position();
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
        if(auto entity = main_scene.Get_if_is_entiety( colision->object_b_id ); main_scene.Get_if_is_food( colision->object_a_id ) && entity)
        {
            entity.value().get().Eat();

            main_scene.Remove_element( colision->object_a_id );
        }
        else if(auto entity = main_scene.Get_if_is_entiety( colision->object_a_id ); entity && main_scene.Get_if_is_food( colision->object_b_id ) )
        {
            entity.value().get().Eat();

            main_scene.Remove_element( colision->object_b_id );
        }
    }
    else if(auto key_pressed = event.Get_if<eruptor::event::Event::Key_pressed>())
    {
        switch(key_pressed->key_type)
        {
            case eruptor::event::Key::R:
                Reload_scene();
                break;
            case eruptor::event::Key::TAB:
                Save_scene();
                break;
            case eruptor::event::Key::M:
                if(mode == Mode::EDITOR)
                {
                    mode = Mode::SIMULATION;
                }
                else if(mode == Mode::SIMULATION)
                {
                    mode = Mode::EDITOR;
                }
                break;
            case eruptor::event::Key::N:

                break;
            case eruptor::event::Key::T:
                if(object_type == Object_type::ENTITY)
                {
                    object_type = Object_type::FOOD;
                }
                else if(object_type == Object_type::FOOD)
                {
                    object_type = Object_type::ENTITY;
                }
                break;
            case eruptor::event::Key::KEY_1:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_1;
                }
                else
                {
                    current_scene_path = scene_path_1;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_2:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_2;
                }
                else
                {
                    current_scene_path = scene_path_2;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_3:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_3;
                }
                else
                {
                    current_scene_path = scene_path_3;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_4:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_4;
                }
                else
                {
                    current_scene_path = scene_path_4;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_5:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_5;
                }
                else
                {
                    current_scene_path = scene_path_5;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_6:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_6;
                }
                else
                {
                    current_scene_path = scene_path_6;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_7:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_7;
                }
                else
                {
                    current_scene_path = scene_path_7;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_8:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_8;
                }
                else
                {
                    current_scene_path = scene_path_8;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            case eruptor::event::Key::KEY_9:
                if(window->Is_key_pressed( eruptor::event::Key::LEFT_SHIFT ))
                {
                    current_simulation_info_path = simulation_path_9;
                }
                else
                {
                    current_scene_path = scene_path_9;
                }
                std::println(std::clog, "Current scene_path: {}\nCurrent simulation_path: {}", current_scene_path.c_str(), current_simulation_info_path.c_str());
                break;
            default:
                break;
        }
    }
}

std::string_view ovum::App::Get_string_from_mode_enum(Mode mode)
{
    switch(mode)
    {
        case Mode::SIMULATION:
            return "Simulation";
        case Mode::EDITOR:
            return "Editor";
    }

    return "None";
}

std::string_view ovum::App::Get_string_from_object_type_enum(Object_type type)
{
    switch(type)
    {
        case Object_type::ENTITY:
            return "Entity";
        case Object_type::FOOD:
            return "Food";
    }

    return "None";
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
