#ifndef OVUM_APP_HPP
#define OVUM_APP_HPP

#include <Eruptor/eruptor.hpp>
#include <Eruptor/scene/scene_parser.hpp>
#include <Eruptor/event/event_manager.hpp>
#include <Ovum/simulation_parser.hpp>
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
    void Update();
    void Update_ai(float delta_time);
    void Render();

    void Reload_scene();

    bool is_running{true};

    GP_communicator gp_comm{};

    std::filesystem::path main_scene_path{"../../scenes/test.papsc"};
    std::filesystem::path simulation_info_path{"../../simulations/test.papsim"};

    ovum::Simulation_scene main_scene{};

    std::random_device random_device{};
    std::mt19937 generator{ random_device() };
    std::uniform_real_distribution<float> rotation_distributor{-glm::half_pi<float>(), glm::half_pi<float>()};
    float time_elapsed{};

    std::chrono::high_resolution_clock app_clock{};
    std::chrono::high_resolution_clock::time_point last_time{};

    glm::vec3 world_min{}, world_max{};

    eruptor::Eruptor engine{};

    eruptor::renderer::Renderer * renderer{};
    eruptor::resource::Resource_manager * resources{};
    eruptor::event::Event_manager & event_manager;
    eruptor::physic::Physic_manager * physic_manager{};

    eruptor::scene::Scene_parser scene_parser{};
    ovum::Simulation_parser simulation_parser{};

    eruptor::hardware::Window * window{};
    eruptor::renderer::Fly_camera * camera{};

    struct Hitbox_loger
    {
        void operator()(const eruptor::physic::Sphere_hitbox & hitbox);
        void operator()(const eruptor::physic::OBB_hitbox & hitbox);
        void operator()(const eruptor::physic::Capsule_hitbox & hitbox);
    } hitbox_loger;
};

}

#endif // OVUM_APP_HPP
