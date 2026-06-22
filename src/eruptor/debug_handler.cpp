#include <Eruptor/debug_handler.hpp>
#include <Eruptor/globals.hpp>
#include <Eruptor/core.hpp>
#include <iostream>


using namespace eruptor;

void eruptor::Debug_handler::Init(Core & core)
{
    if(!enableValidationLayers) return;

    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags{vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                         vk::DebugUtilsMessageSeverityFlagBitsEXT::eError};
    vk::DebugUtilsMessageTypeFlagsEXT message_type_flags{vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation};
    vk::DebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info_ext{};
    debug_utils_messenger_create_info_ext.messageSeverity = severity_flags;
    debug_utils_messenger_create_info_ext.messageType = message_type_flags;
    debug_utils_messenger_create_info_ext.pfnUserCallback = &debug_callback;

    debug_messenger = core.instance.createDebugUtilsMessengerEXT(debug_utils_messenger_create_info_ext);
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL eruptor::Debug_handler::debug_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                                               vk::DebugUtilsMessageTypeFlagsEXT type,
                                                                               const vk::DebugUtilsMessengerCallbackDataEXT * p_callback_data,
                                                                               void * p_user_data)
{
    std::cerr << "validation layer: type " << vk::to_string(type) << "msg: " << p_callback_data->pMessage << '\n';

    return vk::False;
}
