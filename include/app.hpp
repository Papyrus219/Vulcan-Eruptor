#ifndef OVUM_APP_HPP
#define OVUM_APP_HPP

#include <Eruptor/eruptor.hpp>
#include <Eruptor/scene/scene.hpp>
#include <Eruptor/event/event_manager.hpp>

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

    bool is_running{true};

    eruptor::scene::Scene main_scene{};

    std::chrono::high_resolution_clock app_clock{};
    std::chrono::high_resolution_clock::time_point last_time{};

    std::vector< eruptor::resource::Model_handle > models_handles{};

    eruptor::Eruptor engine{};

    eruptor::renderer::Renderer * renderer{};
    eruptor::resource::Resource_manager * resources{};
    eruptor::event::Event_manager & event_manager;

    eruptor::hardware::Window * window{};
    eruptor::renderer::Fly_camera * camera{};
};

}

#endif // OVUM_APP_HPP
