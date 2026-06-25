#ifndef ERUPTOR_HARDWARE_VERTEX_BUFFER_HPP
#define ERUPTOR_HARDWARE_VERTEX_BUFFER_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_raii.hpp>
#include <Eruptor/lib/hardware/vertex.hpp>

namespace eruptor::hardware
{

class Command_manager;
struct Mesh;

class Vertex_buffer
{
public:
    void Init(vma::raii::Allocator & alocator);

    void Start_gather_vertecies();
    void Gather_vertecies(Mesh & mesh);
    void Upload_vertecies(Command_manager & command_manager, vma::raii::Allocator & alocator);

private:
    vma::raii::Buffer vertex_buffer = nullptr;
    vma::raii::Buffer staging_buffer = nullptr;

    std::vector<Vertex> vertecies{};
};

}

#endif // ERUPTOR_HARDWARE_VERTEX_BUFFER_HPP
