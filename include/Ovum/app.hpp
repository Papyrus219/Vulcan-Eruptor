#ifndef OVUM_APP_HPP
#define OVUM_APP_HPP

#include <Eruptor/eruptor.hpp>
#include <Eruptor/scene/scene_parser.hpp>
#include <Eruptor/scene/scene_saver.hpp>
#include <Eruptor/event/event_manager.hpp>
#include <Ovum/simulation_parser.hpp>
#include <Ovum/simulation_saver.hpp>
#include <Ovum/simulation_scene.hpp>
#include <Ovum/gp_communicator.hpp>
#include <random>

namespace ovum
{

class App : public eruptor::event::Event_listener
{
public:
    App();
    void Init();

    void Start_loop();

    virtual void On_event(const eruptor::event::Event & event) override;

private:
    enum class Mode
    {
        EDITOR,
        SIMULATION
    } mode;

    enum class Object_type
    {
        ENTITY,
        FOOD
    } object_type;

    void Update();
    void Update_ai(float delta_time);
    void Render();

    void Reload_scene();
    void Save_scene();

    std::string_view Get_string_from_mode_enum(Mode mode);
    std::string_view Get_string_from_object_type_enum(Object_type type);

    bool is_running{true};

    GP_communicator gp_comm{};

    std::filesystem::path current_scene_path{scene_path_1};
    std::filesystem::path current_simulation_info_path{simulation_path_1};

    ovum::Simulation_scene main_scene{};

    eruptor::resource::Font_handle main_font{};
    eruptor::resource::Font_handle small_font{};

    std::random_device random_device{};
    std::mt19937 generator{ random_device() };
    std::uniform_real_distribution<float> rotation_distributor{-glm::half_pi<float>(), glm::half_pi<float>()};
    float time_elapsed{};

    std::chrono::high_resolution_clock app_clock{};
    std::chrono::high_resolution_clock::time_point last_time{};

    glm::vec3 world_min{}, world_max{};

    uint32_t current_entity_selected{};
    uint32_t current_food_selected{};

    eruptor::Eruptor engine{};

    eruptor::renderer::Renderer * renderer{};
    eruptor::resource::Resource_manager * resources{};
    eruptor::event::Event_manager & event_manager;
    eruptor::physic::Physic_manager * physic_manager{};

    eruptor::scene::Scene_parser scene_parser{};
    ovum::Simulation_parser simulation_parser{};

    eruptor::scene::Scene_saver scene_saver{};
    ovum::Simulation_saver simulation_saver{};

    eruptor::hardware::Window * window{};
    eruptor::renderer::Fly_camera * camera{};

    struct Hitbox_loger
    {
        void operator()(const eruptor::physic::Sphere_hitbox & hitbox);
        void operator()(const eruptor::physic::OBB_hitbox & hitbox);
        void operator()(const eruptor::physic::Capsule_hitbox & hitbox);
    } hitbox_loger;

    static const std::filesystem::path scene_path_1;
    static const std::filesystem::path scene_path_2;
    static const std::filesystem::path scene_path_3;
    static const std::filesystem::path scene_path_4;
    static const std::filesystem::path scene_path_5;
    static const std::filesystem::path scene_path_6;
    static const std::filesystem::path scene_path_7;
    static const std::filesystem::path scene_path_8;
    static const std::filesystem::path scene_path_9;

    static const std::filesystem::path simulation_path_1;
    static const std::filesystem::path simulation_path_2;
    static const std::filesystem::path simulation_path_3;
    static const std::filesystem::path simulation_path_4;
    static const std::filesystem::path simulation_path_5;
    static const std::filesystem::path simulation_path_6;
    static const std::filesystem::path simulation_path_7;
    static const std::filesystem::path simulation_path_8;
    static const std::filesystem::path simulation_path_9;

};

}

#endif // OVUM_APP_HPP
