#include <Eruptor/core.hpp>
#include <Eruptor/globals.hpp>
#include <map>

using namespace eruptor;

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

void eruptor::Core::Setup_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void eruptor::Core::Create_instance()
{
    vk::ApplicationInfo app_info{};
    app_info.pApplicationName = "idk";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Eruptor";
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = vk::ApiVersion14;

    std::vector<char const *> required_layers{};
    if(enableValidationLayers)
    {
        required_layers.assign(validation_layers.begin(), validation_layers.end());
    }

    auto layers_properties = context.enumerateInstanceLayerProperties();
    auto unsuported_layer_it = std::ranges::find_if(required_layers,
    [&layers_properties](auto const & required_layer)
    {
        return std::ranges::none_of(layers_properties,
        [&required_layer](auto const & layer_property)
        {
            return strcmp(layer_property.layerName, required_layer) == 0;
        });
    });
    if(unsuported_layer_it != required_layers.end())
    {
        throw std::runtime_error{"Required validation layer not supported: " + std::string(*unsuported_layer_it)};
    }

    auto requried_extensions = Get_required_instance_extensions();

    auto extension_properties = context.enumerateInstanceExtensionProperties();
    auto unsuported_property_it = std::ranges::find_if(requried_extensions,
    [&extension_properties](auto const & required_extension)
    {
        return std::ranges::none_of(extension_properties,
                [required_extension](auto const &extension_property)
                {
                    return strcmp(extension_property.extensionName, required_extension);
                });
    });
    if(unsuported_property_it != requried_extensions.end())
    {
        throw std::runtime_error{"Required GLFW extension not supported: " + std::string(*unsuported_property_it)};
    }

    vk::InstanceCreateInfo create_info{};
    create_info.pApplicationInfo = &app_info;
    create_info.enabledLayerCount = static_cast<uint32_t>(required_layers.size());
    create_info.ppEnabledLayerNames = required_layers.data();
    create_info.enabledExtensionCount = static_cast<uint32_t>(requried_extensions.size());
    create_info.ppEnabledExtensionNames = requried_extensions.data();

    instance = vk::raii::Instance{context, create_info};
}

void eruptor::Core::Pick_physical_device()
{
    auto physical_devices = instance.enumeratePhysicalDevices();
    if(physical_devices.empty())
    {
        throw std::runtime_error{"failed to find GPUs with Vulkan support!"};
    }

    std::multimap<int, vk::raii::PhysicalDevice> candidates{};
    for(auto & phy_dev : physical_devices)
    {
        auto device_properties = phy_dev.getProperties();
        auto device_features = phy_dev.getFeatures();
        uint32_t score{};

        if(device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
        {
            score += 1000;
        }

        score += device_properties.limits.maxImageDimension2D;

        auto queue_families = physical_device.getQueueFamilyProperties();

        for(auto queue_family : queue_families)
        {
            if((queue_family.queueFlags & vk::QueueFlagBits::eTransfer) && !(queue_family.queueFlags & vk::QueueFlagBits::eGraphics) && !(queue_family.queueFlags & vk::QueueFlagBits::eCompute))
            {
                score += 500;
            }
            else if((queue_family.queueFlags & vk::QueueFlagBits::eTransfer) && !(queue_family.queueFlags & vk::QueueFlagBits::eGraphics))
            {
                score += 250;
            }
        }

        if(!device_features.geometryShader)
        {
            continue;
        }
        candidates.insert(std::make_pair(score, phy_dev));
    }

    if(!candidates.empty() && candidates.rbegin()->first > 0)
    {
        physical_device = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error{"failed to find a suitable GPU!"};
    }
}

std::vector<const char *> eruptor::Core::Get_required_instance_extensions()
{
    uint32_t glfw_extension_count{};
    auto glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
    if(enableValidationLayers)
    {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    return extensions;
}


