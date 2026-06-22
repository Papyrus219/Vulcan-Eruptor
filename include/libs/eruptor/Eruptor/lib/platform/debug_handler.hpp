#ifndef ERUPTOR_DEBUG_HANDLER_HPP
#define ERUPTOR_DEBUG_HANDLER_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace eruptor
{

class Core;

class Debug_handler
{
public:
    void Init(Core & core);
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                           vk::DebugUtilsMessageTypeFlagsEXT type,
                                                           const vk::DebugUtilsMessengerCallbackDataEXT * p_callback_data,
                                                           void * p_user_data);
private:
    vk::raii::DebugUtilsMessengerEXT  debug_messenger = nullptr;
};

}

#endif // ERUPTOR_DEBUG_HANDLER_HPP
