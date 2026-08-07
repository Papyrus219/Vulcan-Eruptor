#ifndef OVUM_SIMULATION_SCENE_HPP
#define OVUM_SIMULATION_SCENE_HPP

#include <Eruptor/scene/scene.hpp>
#include <Ovum/ai_state.hpp>
#include <optional>

namespace ovum
{

struct Entiety_data
{
    void Reset();
    void Eat();

    Ai_state ai_state{};
    float speed{};

    uint32_t render_object_id{};

private:
    uint8_t food_eaten;
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

    std::optional< std::reference_wrapper<Entiety_data> > Get_if_is_entiety(uint32_t id);
    std::optional< std::reference_wrapper<Food_data> > Get_if_is_food(uint32_t id);

    void Remove_element(uint32_t id);

    uint32_t floor{};
    std::vector<Entiety_data> entieties{};
    std::vector<Food_data> food{};
};

}

#endif //OVUM_SIMULATION_SCENE_HPP
