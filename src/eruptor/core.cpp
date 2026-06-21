#include <Eruptor/core.hpp>

using namespace eruptor;

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

void eruptor::Core::Init()
{
    Setup_glfw();
    Create_instance();
}

void eruptor::Core::Test()
{
    Open_window("Test lol", {600, 800});

    while( !glfwWindowShouldClose(windows.back()) )
    {
        if(glfwGetKey(windows.back(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(windows.back(), true);
        }

        glfwPollEvents();
    }
}

void eruptor::Core::Open_window(std::string_view title, glm::uvec2 size)
{
    windows.push_back( glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr) );
}

void eruptor::Core::Create_instance()
{
    vk::ApplicationInfo app_info{};
    app_info.pApplicationName = "idk";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Eruptor";
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = vk::ApiVersion14;

    uint32_t glfw_extension_count{};
    auto glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    auto extension_properties = context.enumerateInstanceExtensionProperties();
    for(uint32_t i{}; i < glfw_extension_count; ++i)
    {
        if(std::ranges::none_of(extension_properties, [glfw_extension = glfw_extensions[i]](auto const & extension_property)
                                                        {
                                                            return strcmp(extension_property.extensionName, glfw_extension) == 0;
                                                        }))
        {
            throw std::runtime_error{"Required GLFW extension not supported: " + std::string{glfw_extensions[i]}};
        }
    }

    vk::InstanceCreateInfo create_info{};
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;



    instance = vk::raii::Instance{context, create_info};
}

void eruptor::Core::Setup_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}
