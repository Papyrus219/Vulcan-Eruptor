#include <Eruptor/lib/hardware/uniform_buffers.hpp>
#include <Eruptor/lib/hardware/device.hpp>
#include <Eruptor/lib/hardware/pipeline.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void eruptor::hardware::Uniform_buffers::Init(Device & device, const uint32_t MAX_FRAMES_IN_FLIGHTS)
{
    Create_descriptor_set_layouts(device.Get_device_handle());
    Create_uniform_buffers(device.Get_alocator_handle(), MAX_FRAMES_IN_FLIGHTS);
    Create_descriptors_pool(device.Get_device_handle(), MAX_FRAMES_IN_FLIGHTS);
    Create_descriptors_sets(device.Get_device_handle(), MAX_FRAMES_IN_FLIGHTS);
}

void eruptor::hardware::Uniform_buffers::Create_descriptor_set_layouts(const vk::raii::Device & device)
{
    vk::DescriptorSetLayoutBinding ubo_mvp_layout_biding{};
    ubo_mvp_layout_biding.setBinding( 0 );
    ubo_mvp_layout_biding.setDescriptorType( vk::DescriptorType::eUniformBuffer );
    ubo_mvp_layout_biding.setDescriptorCount( 1 );
    ubo_mvp_layout_biding.setStageFlags( vk::ShaderStageFlagBits::eVertex );

    vk::DescriptorSetLayoutCreateInfo layout_info{};
    layout_info.setBindings( ubo_mvp_layout_biding );
    descriptor_set_layout = vk::raii::DescriptorSetLayout{device, layout_info};
}

void eruptor::hardware::Uniform_buffers::Create_uniform_buffers(vma::raii::Allocator & alocator, const uint32_t MAX_FRAMES_IN_FLIGHTS)
{
    vk::DeviceSize buffer_size = sizeof(Uniform_buffer_mvp);

    vk::BufferCreateInfo buffer_info{};
    buffer_info.setUsage( vk::BufferUsageFlagBits::eUniformBuffer );
    buffer_info.setSize( buffer_size );
    buffer_info.setSharingMode( vk::SharingMode::eExclusive );

    vma::AllocationCreateInfo alloc_create_info{};
    alloc_create_info.setUsage( vma::MemoryUsage::eAuto );
    alloc_create_info.setFlags( vma::AllocationCreateFlagBits::eHostAccessSequentialWrite | vma::AllocationCreateFlagBits::eMapped );

    vma::AllocationInfo alloc_result{};

    for(uint32_t i{}; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        uniform_mvp_buffers.push_back(alocator.createBuffer( buffer_info, alloc_create_info, alloc_result ));
        uniform_mvp_buffers_mapped.push_back(alloc_result.pMappedData);
    }
}

void eruptor::hardware::Uniform_buffers::Create_descriptors_pool(const vk::raii::Device & device, const uint32_t MAX_FRAMES_IN_FLIGHTS)
{
    vk::DescriptorPoolSize pool_size{};
    pool_size.setType( vk::DescriptorType::eUniformBuffer );
    pool_size.setDescriptorCount(MAX_FRAMES_IN_FLIGHTS);

    vk::DescriptorPoolCreateInfo pool_info{};
    pool_info.setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet );
    pool_info.setMaxSets( MAX_FRAMES_IN_FLIGHTS );
    pool_info.setPoolSizes( pool_size );

    descriptor_pool = vk::raii::DescriptorPool{device, pool_info};
}

void eruptor::hardware::Uniform_buffers::Create_descriptors_sets(const vk::raii::Device & device, const uint32_t MAX_FRAMES_IN_FLIGHTS)
{
    std::vector< vk::DescriptorSetLayout > layouts{MAX_FRAMES_IN_FLIGHTS, *descriptor_set_layout};
    vk::DescriptorSetAllocateInfo alloc_info{};
    alloc_info.setDescriptorPool( descriptor_pool );
    alloc_info.setSetLayouts( layouts );

    descriptor_sets = device.allocateDescriptorSets( alloc_info );

    for(uint32_t i{}; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        vk::DescriptorBufferInfo buffer_info{};
        buffer_info.setBuffer( uniform_mvp_buffers[i] );
        buffer_info.setOffset( 0 );
        buffer_info.setRange( sizeof(Uniform_buffer_mvp) );

        vk::WriteDescriptorSet descriptor_write{};
        descriptor_write.setDstSet( descriptor_sets[i] );
        descriptor_write.setDstBinding( 0 );
        descriptor_write.setDstArrayElement( 0 );
        descriptor_write.setDescriptorCount( 1 );
        descriptor_write.setDescriptorType( vk::DescriptorType::eUniformBuffer );
        descriptor_write.setBufferInfo( buffer_info );

        device.updateDescriptorSets(descriptor_write, {});
    }
}

void eruptor::hardware::Uniform_buffers::Bind_mvp_buffer(vk::raii::CommandBuffer& command_buffer, uint32_t frame_index, Pipeline & pipeline)
{
    hardware::Uniform_buffer_mvp ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(800) / static_cast<float>(600), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy( uniform_mvp_buffers_mapped[frame_index], &ubo, sizeof(ubo));

    command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.Get_pipeline_layout(), 0, *descriptor_sets[frame_index], nullptr);
}


