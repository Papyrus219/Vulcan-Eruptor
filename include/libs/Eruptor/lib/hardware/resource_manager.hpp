#ifndef ERUPTOR_HARDWARE_RESOURCE_MANAGER_HPP
#define ERUPTOR_HARDWARE_RESOURCE_MANAGER_HPP

#include <Eruptor/lib/hardware/mesh.hpp>
#include <Eruptor/lib/hardware/texture.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_raii.hpp>
#include <vector>

namespace eruptor::hardware
{

class Resource_manager
{
public:
    void Init(vma::raii::Allocator & alocator, vk::DeviceSize texture_stage_buffer_size);

private:
    std::vector<Mesh> meshes{};
    std::vector<Texture> textures{};

    vma::raii::Buffer texture_stage_buffer = nullptr;
    void * stage_buffer_mapped_memory{};
};

}

#endif // ERUPTOR_HARDWARE_RESOURCE_MANAGER_HPP
