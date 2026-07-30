#include <Ovum/simulation_saver.hpp>
#include <fstream>
#include <iostream>

using namespace ovum;

std::string_view const ovum::Simulation_saver::error_file{"Simulation saver: Failed to open file to save to!"};

std::expected<void, std::string_view> ovum::Simulation_saver::Save_simulation_data(const Simulation_scene & scene, const std::filesystem::path & path, File_version file_version)
{
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file{path};
    if(!file)
    {
        return std::unexpected{error_file};
    }

    std::println(file, "Version: {}", Get_string_from_file_version(file_version));
    std::println(file, "");

    auto object_alias_it = scene.reverse_object_aliases.find( scene.floor );
    if(object_alias_it != scene.reverse_object_aliases.end())
    {
        std::println(file, "<{}: Floor>", object_alias_it->second);
        std::println(file, "");
    }

    for(auto & entity_data : scene.entieties)
    {
        object_alias_it = scene.reverse_object_aliases.find( entity_data.render_object_id );
        if(object_alias_it != scene.reverse_object_aliases.end())
        {
            std::println(file, "<{}: Entity>", object_alias_it->second);
            std::println(file, "Speed: {}", entity_data.speed);
            std::println(file, "");
        }
    }

    for(auto & food_data : scene.food)
    {
        object_alias_it = scene.reverse_object_aliases.find( food_data.render_object_id );
        if(object_alias_it != scene.reverse_object_aliases.end())
        {
            std::println(file, "<{}: Food>", object_alias_it->second);
            std::println(file, "");
        }
    }

    return {};
}

std::string_view ovum::Simulation_saver::Get_string_from_file_version(File_version file_version)
{
    switch(file_version)
    {
        case File_version::V1_0:
            return "1_0";
    }

    return "";
}
