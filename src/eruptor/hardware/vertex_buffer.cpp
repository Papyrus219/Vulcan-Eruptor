#include <Eruptor/lib/hardware/vertex_buffer.hpp>
#include <Eruptor/lib/hardware/command_manager.hpp>
#include <Eruptor/lib/hardware/mesh.hpp>

void eruptor::hardware::Vertex_buffer::Init(vma::raii::Allocator& alocator)
{
    vk::BufferCreateInfo stage_buffer_info{};
    stage_buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    stage_buffer_info.size = 65536;

    vma::AllocationCreateInfo stage_alloc_info{};
    stage_alloc_info.usage = vma::MemoryUsage::eAuto;
    stage_alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped;

    vk::BufferCreateInfo vertex_create_info{};
    vertex_create_info.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    vertex_create_info.size = 65536;

    vma::AllocationCreateInfo vertex_alocation_info{};
    vertex_alocation_info.usage = vma::MemoryUsage::eAuto;

    vertex_buffer = alocator.createBuffer(vertex_create_info, vertex_alocation_info);
    staging_buffer = alocator.createBuffer(stage_buffer_info, stage_alloc_info);
}

void eruptor::hardware::Vertex_buffer::Start_gather_vertecies()
{
    vertecies.clear();
}

void eruptor::hardware::Vertex_buffer::Gather_vertecies(Mesh & mesh)
{
    mesh.first_vetex_id_in_buffor = vertecies.size();
    for(auto & vertex : mesh.vertecies)
    {
        vertecies.push_back( vertex );
    }
}

void eruptor::hardware::Vertex_buffer::Upload_vertecies(Command_manager & command_manager, vma::raii::Allocator & alocator)
{
    vk::DeviceSize buffer_size = sizeof(vertecies[0]) * vertecies.size();

    void * data_stanging = staging_buffer.getAllocation().map();
    memcpy(data_stanging, vertecies.data(), buffer_size);

    auto command_buffer = command_manager.Begin_transfer_command_record();
    command_buffer.copyBuffer(*staging_buffer, *vertex_buffer, vk::BufferCopy{0, 0, buffer_size});
    command_manager.End_transfer_command_record( std::move(command_buffer) );

    vertecies.clear();
}
