#ifndef ERUPTOR_RESOURCE_MODEL_HPP
#define ERUPTOR_RESOURCE_MODEL_HPP

#include <Eruptor/lib/resource/resource_handle.hpp>

#include <vector>

namespace eruptor::resource
{

enum class Status
{
    UNINITIALIZED,
    PENDING,
    LODADED,
    ERROR
};

struct Model
{
    Status status{};

    std::vector<Mesh_handle> Meshes_handles{};
    std::vector<Material_handle> materials_handles{};
};

}

#endif //ERUPTOR_RESOURCES_MODEL_HPP
