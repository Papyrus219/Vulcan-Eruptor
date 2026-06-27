#ifndef ERUPTOR_HARDWARE_RESOURCE_MANAGER_HPP
#define ERUPTOR_HARDWARE_RESOURCE_MANAGER_HPP

#include <Eruptor/lib/hardware/resources/mesh.hpp>
#include <Eruptor/lib/hardware/resources/texture.hpp>
#include <Eruptor/lib/hardware/resources/geometry_buffer.hpp>
#include <Eruptor/lib/hardware/resources/vertex.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_raii.hpp>
#include <vector>

namespace eruptor::hardware
{

class Command_manager;
class Device;
struct Mesh_data;
struct Texture_data;

class Resource_manager
{
public:
    void Init(vma::raii::Allocator & allocator, vk::DeviceSize max_texture_buffor_size, vk::DeviceSize max_vertex_buffor_size, vk::DeviceSize max_index_buffor_size);

    void Assign_command_manager(Command_manager & command_manager);
    void Assign_device(Device & device);

    uint32_t Stage_mesh_data(Mesh_data & mesh_data);
    uint32_t Stage_texture_data(Texture_data & texture_data);

private:
    std::vector<Mesh> meshes{};
    std::vector<Texture> textures{};

    vma::raii::Buffer geometry_staging_buffer = nullptr;
    vma::raii::Buffer texture_stage_buffer = nullptr;

    void * geometry_stage_mapped_data{};
    void * texture_stage_mapped_memory{};

    vk::raii::Fence vertex_stage_fence = nullptr;
    vk::raii::Fence index_stage_fence = nullptr;
    std::vector<vk::raii::Fence> textures_stage_fences{};

    vk::DeviceSize max_texture_buffor_size{};
    vk::DeviceSize max_vertex_buffor_size{};
    vk::DeviceSize max_index_buffor_size{};

    vk::DeviceSize curr_vertex_offset{};
    vk::DeviceSize curr_index_offset{};
    vk::DeviceSize curr_texture_offset{};

    Device * device;
    Command_manager * command_manager;
};

struct Mesh_data
{
    void Clear();

    std::vector<Vertex> vertecies{};
    std::vector<uint32_t> indices{};
    uint32_t texture_id{};
};

struct Texture_data
{
    int width{};
    int height{};
    int tex_chanels{};
    unsigned char * pixels{};
};

}

#endif // ERUPTOR_HARDWARE_RESOURCE_MANAGER_HPP
