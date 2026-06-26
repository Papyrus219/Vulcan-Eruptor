#include <Eruptor/lib/hardware/resource_manager.hpp>

void eruptor::hardware::Resource_manager::Init(vma::raii::Allocator & alocator, vk::DeviceSize texture_stage_buffer_size)
{
    vk::BufferCreateInfo stage_buffer_info{};
    stage_buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    stage_buffer_info.size = texture_stage_buffer_size;
    stage_buffer_info.sharingMode = vk::SharingMode::eExclusive;

    vma::AllocationCreateInfo stage_alloc_info{};
    stage_alloc_info.usage = vma::MemoryUsage::eAuto;
    stage_alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped;

    vma::AllocationInfo stage_alloc_result{};

    texture_stage_buffer = alocator.createBuffer(stage_buffer_info, stage_alloc_info, stage_alloc_result);
    stage_buffer_mapped_memory = stage_alloc_result.pMappedData;
}
