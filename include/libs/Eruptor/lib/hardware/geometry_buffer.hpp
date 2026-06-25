#ifndef ERUPTOR_HARDWARE_GEOMETRY_BUFFER_HPP
#define ERUPTOR_HARDWARE_GEOMETRY_BUFFER_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_raii.hpp>
#include <Eruptor/lib/hardware/vertex.hpp>

namespace eruptor::hardware
{

class Command_manager;
struct Mesh;

class Geometry_buffer
{
public:
    void Init(vma::raii::Allocator & alocator, vk::DeviceSize max_vertex_buffor_size, vk::DeviceSize max_index_buffor_size);

    void Bind(vk::raii::CommandBuffer & command_buffer);

    void Gather_geometry(Mesh & mesh);
    void Upload_geometry(Command_manager & command_manager);

private:
    vma::raii::Buffer vertex_buffer = nullptr;
    vma::raii::Buffer index_buffer = nullptr;
    vma::raii::Buffer staging_buffer = nullptr;

    vk::DeviceSize max_vertex_buffor_size{};
    vk::DeviceSize max_index_buffor_size{};
    void * stanging_mapped_data{};

    std::vector<Vertex> vertecies{};
    std::vector<uint32_t> indices{};
};

}

#endif // ERUPTOR_HARDWARE_VERTEX_BUFFER_HPP
