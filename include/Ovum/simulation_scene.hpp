#ifndef OVUM_SIMULATION_SCENE_HPP
#define OVUM_SIMULATION_SCENE_HPP

#include <Eruptor/scene/scene.hpp>
#include <Ovum/ai_state.hpp>

namespace ovum
{

struct Entiety_data
{
    Ai_state ai_state{};
    float speed{};

    uint32_t render_object_id{};
};

struct Food_data
{
    uint32_t render_object_id{};
};

struct Simulation_scene: public eruptor::scene::Scene
{
    Simulation_scene() = default;

    Simulation_scene(const Simulation_scene & other);
    Simulation_scene(Simulation_scene && other);
    Simulation_scene(const eruptor::scene::Scene & other);
    Simulation_scene(eruptor::scene::Scene && other);

    Simulation_scene & operator=(const Simulation_scene & other);
    Simulation_scene & operator=(Simulation_scene && other);
    Simulation_scene & operator=(const eruptor::scene::Scene & other);
    Simulation_scene & operator=(eruptor::scene::Scene && other);

    uint32_t floor{};
    std::vector<Entiety_data> entieties{};
    std::vector<Food_data> food{};
};

}

#endif //OVUM_SIMULATION_SCENE_HPP
