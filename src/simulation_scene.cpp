#include <Ovum/simulation_scene.hpp>
#include <iostream>
#include <print>

ovum::Simulation_scene::Simulation_scene(const Simulation_scene & other): eruptor::scene::Scene{ other }, floor{ other.floor }, entieties{ other.entieties }, food{ other.food }
{

}

ovum::Simulation_scene::Simulation_scene(Simulation_scene && other): eruptor::scene::Scene{ std::move( other ) }, floor{ other.floor }, entieties{ std::move( other.entieties ) }, food{ std::move( other.food ) }
{

}

ovum::Simulation_scene::Simulation_scene(const eruptor::scene::Scene & other): eruptor::scene::Scene{ other }
{

}

ovum::Simulation_scene::Simulation_scene(eruptor::scene::Scene && other): eruptor::scene::Scene{ std::move( other ) }
{

}

ovum::Simulation_scene & ovum::Simulation_scene::operator=(const Simulation_scene & other)
{
    eruptor::scene::Scene::operator=( other );

    this->floor = other.floor;
    this->entieties = other.entieties;
    this->food = other.food;

    return *this;
}

ovum::Simulation_scene & ovum::Simulation_scene::operator=(Simulation_scene && other)
{
    eruptor::scene::Scene::operator=( std::move( other ) );

    this->floor = std::move( other.floor );
    this->entieties = std::move( other.entieties );
    this->food = std::move( other.food );

    return *this;
}

ovum::Simulation_scene & ovum::Simulation_scene::operator=(const eruptor::scene::Scene & other)
{
    eruptor::scene::Scene::operator=( std::move( other ) );

    return *this;
}

ovum::Simulation_scene & ovum::Simulation_scene::operator=(eruptor::scene::Scene && other)
{
    eruptor::scene::Scene::operator=( std::move( other ) );

    return *this;
}

std::optional< std::reference_wrapper<ovum::Entiety_data> > ovum::Simulation_scene::Get_if_is_entiety(uint32_t id)
{
    for(auto i{0UZ}; i < entieties.size(); i++)
    {
        if(entieties[i].render_object_id == id)
        {
            return entieties[i];
        }
    }

    return std::nullopt;
}

std::optional< std::reference_wrapper<ovum::Food_data> > ovum::Simulation_scene::Get_if_is_food(uint32_t id)
{
    for(auto i{0UZ}; i < food.size(); i++)
    {
        if(food[i].render_object_id == id)
        {
            return food[i];
        }
    }

    return std::nullopt;
}

void ovum::Simulation_scene::Remove_element(uint32_t id)
{
    render_objects.erase( render_objects.begin() + id );

    if(floor == id) floor = 0;

    for(auto & entity : entieties)
    {
        if( entity.render_object_id == id)
        {
            entity.render_object_id = 0;
            return;
        }
    }

    for(auto & food : entieties)
    {
        if( food.render_object_id == id )
        {
            food.render_object_id = 0;
            return;
        }
    }
}

void ovum::Entiety_data::Reset()
{
    food_eaten = 0;
}

void ovum::Entiety_data::Eat()
{
    std::println(std::clog, "Omniommnom!");

    food_eaten++;
}


