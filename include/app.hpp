#ifndef OVUM_APP_HPP
#define OVUM_APP_HPP

#include <Eruptor/eruptor.hpp>
#include <Eruptor/scene/scene_parser.hpp>
#include <Eruptor/scene/scene.hpp>
#include <Eruptor/event/event_manager.hpp>
#include <gp_communicator.hpp>

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
    void Render();

    void Reload_scene();

    bool is_running{true};

    GP_communicator gp_comm{};

    std::filesystem::path main_scene_path{"../../scenes/test.papsc"};
    eruptor::scene::Scene main_scene{};

    std::chrono::high_resolution_clock app_clock{};
    std::chrono::high_resolution_clock::time_point last_time{};

    std::vector< eruptor::resource::Model_handle > models_handles{};

    eruptor::Eruptor engine{};

    eruptor::renderer::Renderer * renderer{};
    eruptor::resource::Resource_manager * resources{};
    eruptor::event::Event_manager & event_manager;
    eruptor::physic::Physic_manager * physic_manager{};

    eruptor::scene::Scene_parser scene_parser{};

    eruptor::hardware::Window * window{};
    eruptor::renderer::Fly_camera * camera{};

    struct Hitbox_loger
    {
        void operator()(const eruptor::physic::Sphere_hitbox & hitbox);
        void operator()(const eruptor::physic::OBB_hitbox & hitbox);
    } hitbox_loger;
};

}

#endif // OVUM_APP_HPP
