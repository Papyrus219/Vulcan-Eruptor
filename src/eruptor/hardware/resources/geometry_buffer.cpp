#include <Eruptor/lib/hardware/resources/geometry_buffer.hpp>
#include <Eruptor/lib/hardware/command_manager.hpp>
#include <Eruptor/lib/hardware/resources/mesh.hpp>

void eruptor::hardware::Geometry_buffer::Init()
{
    this->max_vertex_buffor_size = max_vertex_buffor_size;
    this->max_index_buffor_size = max_index_buffor_size;

    vk::BufferCreateInfo stage_buffer_info{};
    stage_buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    stage_buffer_info.size = max_vertex_buffor_size + max_index_buffor_size;
    stage_buffer_info.sharingMode = vk::SharingMode::eExclusive;

    vma::AllocationCreateInfo stage_alloc_info{};
    stage_alloc_info.usage = vma::MemoryUsage::eAuto;
    stage_alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped;

    vma::AllocationInfo stage_alloc_result{};


    stanging_mapped_data = stage_alloc_result.pMappedData;
}

void eruptor::hardware::Geometry_buffer::Bind(vk::raii::CommandBuffer & command_buffer)
{
    command_buffer.bindVertexBuffers(0, *vertex_buffer, {0});
    command_buffer.bindIndexBuffer(*index_buffer, 0, vk::IndexType::eUint32);
}

void eruptor::hardware::Geometry_buffer::Gather_geometry(Mesh & mesh)
{
    mesh.vertex_offset = vertecies.size();
    mesh.vertex_amount = mesh.vertecies.size();
    mesh.indices_offset = indices.size();
    mesh.indices_amount = mesh.indices.size();

    vertecies.insert( vertecies.end(), mesh.vertecies.begin(), mesh.vertecies.end() );
    indices.insert( indices.end(), mesh.indices.begin(), mesh.indices.end() );

    mesh.vertecies.clear();
    mesh.indices.clear();

    mesh.vertecies.shrink_to_fit();
    mesh.indices.shrink_to_fit();
}

void eruptor::hardware::Geometry_buffer::Upload_geometry(Command_manager & command_manager)
{
    vk::DeviceSize vertex_buffor_size = sizeof(vertecies[0]) * vertecies.size();
    vk::DeviceSize index_buffor_size = sizeof(indices[0]) * indices.size();

    if(vertex_buffor_size > max_vertex_buffor_size || index_buffor_size > max_index_buffor_size)
    {
        throw std::runtime_error{"Geometry buffer overflow!"};
    }

    memcpy(stanging_mapped_data, vertecies.data(), vertex_buffor_size);
    memcpy((char*)(stanging_mapped_data) + max_vertex_buffor_size, indices.data(), index_buffor_size);

    auto command_buffer = command_manager.Begin_transfer_command_record();
    command_buffer.copyBuffer(*staging_buffer, *vertex_buffer, vk::BufferCopy{0, 0, vertex_buffor_size});
    command_buffer.copyBuffer(*staging_buffer, *index_buffer, vk::BufferCopy(max_vertex_buffor_size, 0, index_buffor_size));
    command_manager.End_transfer_command_record( std::move(command_buffer) );

    vertecies.clear();
    indices.clear();
}
