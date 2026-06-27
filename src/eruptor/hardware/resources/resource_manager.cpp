#include <Eruptor/lib/hardware/resources/resource_manager.hpp>
#include <Eruptor/lib/hardware/command_manager.hpp>
#include <Eruptor/lib/hardware/device.hpp>

void eruptor::hardware::Resource_manager::Init(vma::raii::Allocator & allocator, vk::DeviceSize max_texture_buffor_size, vk::DeviceSize max_vertex_buffor_size, vk::DeviceSize max_index_buffor_size)
{
    this->max_texture_buffor_size = max_texture_buffor_size;
    this->max_vertex_buffor_size = max_vertex_buffor_size;
    this->max_index_buffor_size = max_index_buffor_size;

    vk::BufferCreateInfo stage_buffer_info{};
    stage_buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    stage_buffer_info.size = max_texture_buffor_size;
    stage_buffer_info.sharingMode = vk::SharingMode::eExclusive;

    vma::AllocationCreateInfo stage_alloc_info{};
    stage_alloc_info.usage = vma::MemoryUsage::eAuto;
    stage_alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped;

    vma::AllocationInfo stage_alloc_result{};

    texture_stage_buffer = allocator.createBuffer(stage_buffer_info, stage_alloc_info, stage_alloc_result);
    texture_stage_mapped_memory = stage_alloc_result.pMappedData;

    stage_buffer_info.size = max_vertex_buffor_size + max_index_buffor_size;

    geometry_staging_buffer = allocator.createBuffer(stage_buffer_info, stage_alloc_info, stage_alloc_result);
    geometry_stage_mapped_data = stage_alloc_result.pMappedData;

    vk::FenceCreateInfo fence_info{};
    fence_info.flags = vk::FenceCreateFlagBits::eSignaled;
    vertex_stage_fence = vk::raii::Fence{device->Get_device_handle(), fence_info};
    index_stage_fence = vk::raii::Fence{device->Get_device_handle(), fence_info};
}

void eruptor::hardware::Resource_manager::Assign_command_manager(Command_manager & command_manager)
{
    this->command_manager = &command_manager;
}

void eruptor::hardware::Resource_manager::Assign_device(Device & device)
{
    this->device = &device;
}

uint32_t eruptor::hardware::Resource_manager::Stage_mesh_data(Mesh_data & mesh_data)
{
    if(curr_vertex_offset + (mesh_data.vertecies.size() * sizeof(Vertex)) > max_vertex_buffor_size)
    {
        throw std::runtime_error{"ERROR::RESOURCE_MANAGER::Vertex stage buffer overflow."};
    }
    if(curr_index_offset + (mesh_data.indices.size() * sizeof(uint32_t)) > max_index_buffor_size)
    {
        throw std::runtime_error{"ERROR::RESOURCE_MANAGER::Index stage buffer overflow."};
    }

    memcpy(reinterpret_cast<char *>(geometry_stage_mapped_data) + curr_vertex_offset, mesh_data.vertecies.data(), sizeof(Vertex) * mesh_data.vertecies.size() );
    memcpy(reinterpret_cast<char *>(geometry_stage_mapped_data) + max_vertex_buffor_size + curr_index_offset, mesh_data.indices.data(), sizeof(uint32_t) * mesh_data.indices.size());

    Mesh tmp_mesh{};
    tmp_mesh.vertex_offset = curr_vertex_offset / sizeof(Vertex);
    tmp_mesh.indices_offset = curr_index_offset / sizeof(uint32_t);
    tmp_mesh.indices_amount = mesh_data.indices.size();
    tmp_mesh.vertex_amount = mesh_data.vertecies.size();
    tmp_mesh.texture_id = mesh_data.texture_id;

    meshes.push_back( std::move(tmp_mesh) );

    curr_vertex_offset += (mesh_data.vertecies.size() * sizeof(Vertex));
    curr_index_offset += (mesh_data.indices.size() * sizeof(uint32_t));

    return meshes.size() - 1;
}

uint32_t eruptor::hardware::Resource_manager::Stage_texture_data(Texture_data & texture_data)
{
    vk::DeviceSize image_size = texture_data.width * texture_data.height * 4;

    if(curr_texture_offset + image_size > max_texture_buffor_size)
    {
        throw std::runtime_error{"ERROR::RESOURCE_MANAGER::Texture stage buffer overflow."};
    }

    memcpy(reinterpret_cast<char *>(texture_stage_mapped_memory) + curr_texture_offset, texture_data.pixels, image_size);

    Texture tmp_tex{};
    tmp_tex.Init(*device, texture_data.width, texture_data.height, curr_texture_offset);

    textures.push_back( std::move(tmp_tex) );

    curr_texture_offset += image_size;

    vk::FenceCreateInfo fence_info{};
    fence_info.flags = vk::FenceCreateFlagBits::eSignaled;
    textures_stage_fences.emplace_back(device->Get_device_handle(), fence_info);

    return textures.size() - 1;
}

void eruptor::hardware::Mesh_data::Clear()
{
    vertecies.clear();
    indices.clear();
}
