#ifndef ERUPTOR_HARDWARE_COMMAND_MANAGER_HPP
#define ERUPTOR_HARDWARE_COMMAND_MANAGER_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vector>

namespace eruptor::hardware
{

class Device;

class Command_manager
{
public:
    void Init(Device & device, int frames_amount);

    vk::raii::CommandBuffer & Begin_graphic_command_record(int current_frame);
    vk::raii::CommandBuffer & Begin_compute_command_record(int current_frame);
    vk::raii::CommandBuffer Begin_transfer_command_record();

    void End_graphic_command_record(vk::raii::CommandBuffer & command_buffer);
    void End_compute_command_record(vk::raii::CommandBuffer & command_buffer);
    void End_transfer_command_record(vk::raii::CommandBuffer && command_buffer);

private:
    Device * device{};

    std::vector<vk::raii::CommandBuffer> graphics_command_buffers{};
    std::vector<vk::raii::CommandBuffer> compute_command_buffers{};

    vk::raii::CommandPool compute_command_pool = nullptr;
    vk::raii::CommandPool graphic_command_pool = nullptr;
    vk::raii::CommandPool transfer_command_pool = nullptr;
};

}

#endif // ERUPTOR_HARDWARE_COMMAND_MANAGER_HPP
