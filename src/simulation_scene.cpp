#include <Ovum/simulation_scene.hpp>

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
