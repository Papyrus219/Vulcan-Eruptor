#include <Eruptor/lib/platform/device.hpp>
#include <Eruptor/lib/platform/core.hpp>
#include <Eruptor/lib/platform/utility_structures.hpp>
#include <map>
#include <set>

void eruptor::platform::Device::Init(Core& core)
{
    Pick_physical_device(core);
    Create_logical_device(core);
}

void eruptor::platform::Device::Pick_physical_device(Core & core)
{
    auto physical_devices = core.Get_physical_devices();
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

        if(!Is_device_sutiable(phy_dev))
        {
            continue;
        }

        if(device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
        {
            score += 1000;
        }

        score += device_properties.limits.maxImageDimension2D;

        auto queue_families = phy_dev.getQueueFamilyProperties();

        uint32_t best_transfer_score{};
        uint32_t best_compute_score{};

        for(auto queue_family : queue_families)
        {
            if((queue_family.queueFlags & vk::QueueFlagBits::eTransfer) )
            {
                if(!(queue_family.queueFlags & vk::QueueFlagBits::eGraphics) && !(queue_family.queueFlags & vk::QueueFlagBits::eCompute))
                {
                    best_transfer_score = std::max(best_transfer_score, 500u);
                }
                else if(!(queue_family.queueFlags & vk::QueueFlagBits::eGraphics))
                {
                    best_transfer_score = std::max(best_transfer_score, 250u);
                }
            }

            if((queue_family.queueFlags & vk::QueueFlagBits::eCompute))
            {
                if(!(queue_family.queueFlags & vk::QueueFlagBits::eGraphics) && !(queue_family.queueFlags & vk::QueueFlagBits::eTransfer))
                {
                    best_compute_score = std::max(best_compute_score, 500u);
                }
                else if(!(queue_family.queueFlags & vk::QueueFlagBits::eGraphics))
                {
                    best_compute_score = std::max(best_compute_score, 250u);
                }
            }
        }
        score += best_transfer_score + best_compute_score;

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

void eruptor::platform::Device::Create_logical_device(Core & core)
{
    std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties();

    Queue_fam_choise transfer_choise{};
    Queue_fam_choise compute_choise{};

    for(uint32_t i{}; i < queue_family_properties.size(); i++)
    {
        if((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics))
        {
            graphics_index = i;
        }

        if((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eTransfer))
        {
            uint32_t score{};
            if(!(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eCompute))
            {
                score = 100;
            }
            else if(!(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics))
            {
                score = 50;
            }
            else
            {
                score = 25;
            }

            if(transfer_choise.score <= score)
            {
                transfer_choise.index = i;
                transfer_choise.score = score;
            }

            if((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eCompute))
            {
                uint32_t score{};
                if(!(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eTransfer))
                {
                    score = 100;
                }
                else if(!(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics))
                {
                    score = 50;
                }
                else
                {
                    score = 25;
                }

                if(compute_choise.score <= score)
                {
                    compute_choise.index = i;
                    compute_choise.score = score;
                }
            }
        }
    }

    transfer_index = transfer_choise.index;
    compute_index = compute_choise.index;

    std::set<uint32_t> unique_queues{};
    unique_queues.insert(graphics_index);
    unique_queues.insert(transfer_index);
    unique_queues.insert(compute_index);

    std::vector<vk::DeviceQueueCreateInfo> device_queue_create_infos{};

    float priority{1.0f};
    for(auto queue_index : unique_queues)
    {
        vk::DeviceQueueCreateInfo tmp_info{};
        tmp_info.queueFamilyIndex = queue_index;
        tmp_info.queueCount = 1;
        tmp_info.pQueuePriorities = &priority;

        device_queue_create_infos.push_back(tmp_info);
    }

    vk::StructureChain<vk::PhysicalDeviceFeatures2,
    vk::PhysicalDeviceVulkan11Features,
    vk::PhysicalDeviceVulkan13Features,
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
    feature_chain;
    feature_chain.get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters = true;
    feature_chain.get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering = true;
    feature_chain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState = true;

    std::vector<const char *> required_device_extension{vk::KHRSwapchainExtensionName};

    vk::DeviceCreateInfo device_create_info{};
    device_create_info.pNext = &feature_chain.get<vk::PhysicalDeviceFeatures2>();
    device_create_info.queueCreateInfoCount = static_cast<uint32_t>(device_queue_create_infos.size());
    device_create_info.pQueueCreateInfos = device_queue_create_infos.data();
    device_create_info.enabledExtensionCount = static_cast<uint32_t>(required_device_extension.size());
    device_create_info.ppEnabledExtensionNames = required_device_extension.data();

    device = vk::raii::Device{physical_device, device_create_info};

    graphics_queue = vk::raii::Queue{device, graphics_index, 0};
    transfer_queue = vk::raii::Queue{device, transfer_index, 0};
    compute_queue = vk::raii::Queue{device, compute_index, 0};
}

bool eruptor::platform::Device::Is_device_sutiable(const vk::raii::PhysicalDevice & device)
{
    auto device_properties = device.getProperties();
    auto device_features = device.getFeatures();

    bool has_geometry_shader = device_features.geometryShader;
    std::vector<const char*> required_device_extension = {vk::KHRSwapchainExtensionName};

    auto available_device_extensions = device.enumerateDeviceExtensionProperties();
    bool supports_all_required_extensions =
    std::ranges::all_of( required_device_extension,
                         [&available_device_extensions]( auto const & required_device_extension )
                         {
                             return std::ranges::any_of( available_device_extensions,
                                                         [required_device_extension]( auto const & available_device_extensions )
                                                         { return strcmp( available_device_extensions.extensionName, required_device_extension ) == 0; } );
                         } );

    auto queue_families = device.getQueueFamilyProperties();
    bool supports_graphics =
    std::ranges::any_of(queue_families, [](auto const &qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

    auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2,
    vk::PhysicalDeviceVulkan11Features,
    vk::PhysicalDeviceVulkan13Features,
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

    bool supports_required_features = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
    features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
    features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

    return has_geometry_shader && supports_all_required_extensions && supports_graphics && supports_required_features;
}
