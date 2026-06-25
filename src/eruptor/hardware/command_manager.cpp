#include <Eruptor/lib/hardware/command_manager.hpp>
#include <Eruptor/lib/hardware/device.hpp>

void eruptor::hardware::Command_manager::Init(Device & device, int frames_amount)
{
    this->device = &device;

    vk::CommandPoolCreateInfo graphics_pool_info{};
    vk::CommandPoolCreateInfo transfer_pool_info{};
    vk::CommandPoolCreateInfo compute_pool_info{};

    graphics_pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    graphics_pool_info.queueFamilyIndex = device.queues.Get_graphics_queue_index();

    transfer_pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    transfer_pool_info.queueFamilyIndex = device.queues.Get_transfer_queue_index();

    compute_pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    compute_pool_info.queueFamilyIndex = device.queues.Get_compute_queue_index();

    graphic_command_pool = vk::raii::CommandPool{device.Get_device_handle(), graphics_pool_info};
    transfer_command_pool = vk::raii::CommandPool{device.Get_device_handle(), transfer_pool_info};
    compute_command_pool = vk::raii::CommandPool{device.Get_device_handle(), compute_pool_info};

    vk::CommandBufferAllocateInfo graphic_command_buffer_allocate_info{};
    vk::CommandBufferAllocateInfo compute_command_buffer_allocate_info{};

    graphic_command_buffer_allocate_info.commandPool = graphic_command_pool;
    graphic_command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
    graphic_command_buffer_allocate_info.commandBufferCount = frames_amount;

    compute_command_buffer_allocate_info.commandPool = compute_command_pool;
    compute_command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
    compute_command_buffer_allocate_info.commandBufferCount = frames_amount;

    graphics_command_buffers = vk::raii::CommandBuffers{device.Get_device_handle(), graphic_command_buffer_allocate_info};
    compute_command_buffers = vk::raii::CommandBuffers{device.Get_device_handle(), compute_command_buffer_allocate_info};
}

vk::raii::CommandBuffer & eruptor::hardware::Command_manager::Begin_graphic_command_record(int current_frame)
{
    graphics_command_buffers[current_frame].begin({});

    return graphics_command_buffers[current_frame];
}

vk::raii::CommandBuffer & eruptor::hardware::Command_manager::Begin_compute_command_record(int current_frame)
{
    compute_command_buffers[current_frame].begin({});

    return compute_command_buffers[current_frame];
}

vk::raii::CommandBuffer eruptor::hardware::Command_manager::Begin_transfer_command_record()
{
    vk::CommandBufferAllocateInfo transfer_command_buffer_allocate_info{};
    transfer_command_buffer_allocate_info.commandPool = transfer_command_pool;
    transfer_command_buffer_allocate_info.level = vk::CommandBufferLevel::ePrimary;
    transfer_command_buffer_allocate_info.commandBufferCount = 1;

    vk::raii::CommandBuffer command_buffer = std::move( vk::raii::CommandBuffers(device->Get_device_handle(), transfer_command_buffer_allocate_info).front() );

    vk::CommandBufferBeginInfo begin_info{};
    begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    command_buffer.begin(begin_info);

    return command_buffer;
}

void eruptor::hardware::Command_manager::End_graphic_command_record(vk::raii::CommandBuffer & command_buffer)
{
    vk::SubmitInfo submit_info{};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &*command_buffer;

    command_buffer.end();
    device->queues.Get_graphics_queue_handle().submit(submit_info, nullptr);
}

void eruptor::hardware::Command_manager::End_compute_command_record(vk::raii::CommandBuffer& command_buffer)
{
    vk::SubmitInfo submit_info{};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &*command_buffer;

    command_buffer.end();
    device->queues.Get_compute_queue_handle().submit(submit_info, nullptr);
}

void eruptor::hardware::Command_manager::End_transfer_command_record(vk::raii::CommandBuffer && command_buffer)
{
    vk::SubmitInfo submit_info{};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &*command_buffer;

    command_buffer.end();
    device->queues.Get_graphics_queue_handle().submit(submit_info, nullptr);
    device->queues.Get_graphics_queue_handle().waitIdle();
}
