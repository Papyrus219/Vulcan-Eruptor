#ifndef OVUM_SIMULATION_SAVER_HPP
#define OVUM_SIMULATION_SAVER_HPP

#include <Ovum/simulation_scene.hpp>
#include <expected>

namespace ovum
{

class Simulation_saver
{
public:
    enum class File_version
    {
        V1_0,
    };

    std::expected<void, std::string_view> Save_simulation_data(const Simulation_scene & scene, const std::filesystem::path & path, File_version file_version = File_version::V1_0);

private:
    std::string_view Get_string_from_file_version(File_version file_version);

    static const std::string_view error_file;
};

}

#endif // OVUM_SIMULATION_SAVER_HPP
