#ifndef OVUM_SIMULATION_SCENE_HPP
#define OVUM_SIMULATION_SCENE_HPP

#include <Eruptor/scene/scene.hpp>
#include <Eruptor/resource_manager.hpp>
#include <Eruptor/resource/resource_handle.hpp>
#include <Ovum/ai_state.hpp>
#include <queue>
#include <optional>
#include <cstdint>

namespace ovum
{

struct Entiety_data
{
    void Reset();
    void Eat();

    Ai_data ai_data{};
    float speed{};
    float energy{30};

    uint32_t render_object_id{};

    uint8_t food_eaten;
};

struct Food_data
{
    uint32_t render_object_id{};
};

struct Simulation_scene: public eruptor::scene::Scene
{
    Simulation_scene() = default;

    void Init(eruptor::resource::Resource_manager & resource_manager);

    Simulation_scene(const Simulation_scene & other);
    Simulation_scene(Simulation_scene && other);
    Simulation_scene(const eruptor::scene::Scene & other);
    Simulation_scene(eruptor::scene::Scene && other);

    Simulation_scene & operator=(const Simulation_scene & other);
    Simulation_scene & operator=(Simulation_scene && other);
    Simulation_scene & operator=(const eruptor::scene::Scene & other);
    Simulation_scene & operator=(eruptor::scene::Scene && other);

    uint32_t Add_entity();
    uint32_t Add_food();

    void Remove_entity(uint32_t id);
    void Remove_food(uint32_t id);

    std::optional< std::reference_wrapper<Entiety_data> > Get_if_is_entiety(uint32_t id);
    std::optional< std::reference_wrapper<Food_data> > Get_if_is_food(uint32_t id);

    void Remove_element(uint32_t id);

    std::queue<uint32_t> free_objects{};

    uint32_t floor{};
    std::vector<Entiety_data> entieties{};
    std::vector<Food_data> food{};

    eruptor::resource::Model_handle blob_handle{};
    eruptor::resource::Model_handle food_handle{};

    uint32_t next_entity_alias_id{};
    uint32_t next_food_alias_id{};

    eruptor::resource::Resource_manager * resource_manager{};
};

}

#endif //OVUM_SIMULATION_SCENE_HPP
