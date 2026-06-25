#ifndef ERUPTOR_HARDWARE_MESH_HPP
#define ERUPTOR_HARDWARE_MESH_HPP

#include <Eruptor/lib/hardware/vertex.hpp>
#include <vector>

namespace eruptor::hardware
{

struct Mesh
{
    std::vector<Vertex> vertecies{};
    std::vector<uint32_t> indices{};

    uint32_t first_vetex_id_in_buffor{};
};

}

#endif // ERUPTOR_HARDWARE_MESH_HPP
