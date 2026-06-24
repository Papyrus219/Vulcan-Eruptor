#include <Eruptor/lib/hardware/pipeline.hpp>
#include <Eruptor/lib/hardware/device.hpp>
#include <Eruptor/lib/hardware/swapchain.hpp>
#include <Eruptor/lib/hardware/vertex.hpp>
#include <fstream>

void eruptor::hardware::Pipeline::Init(Device& device, Swapchain & swapchain)
{
    auto swapchain_extent = swapchain.Get_extent();
    Create_graphics_pipeline(device, swapchain_extent);
}

void eruptor::hardware::Pipeline::Create_graphics_pipeline(Device& device, vk::Extent2D & swapchain_extent)
{
    vk::raii::ShaderModule shader_module = Create_shader_module(device, Read_file("./shaders/shader.slang.spv"));

    vk::PipelineShaderStageCreateInfo vertex_shader_stage_info{};
    vertex_shader_stage_info.stage = vk::ShaderStageFlagBits::eVertex;
    vertex_shader_stage_info.module = shader_module;
    vertex_shader_stage_info.pName = "Vert_main";

    vk::PipelineShaderStageCreateInfo fragment_shader_stage_info{};
    fragment_shader_stage_info.stage = vk::ShaderStageFlagBits::eFragment;
    fragment_shader_stage_info.module = shader_module;
    fragment_shader_stage_info.pName = "Frag_main";

    vk::PipelineShaderStageCreateInfo shader_stages[] = {vertex_shader_stage_info, fragment_shader_stage_info };

    std::vector<vk::DynamicState> dynamic_states = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo dynamic_state_info{};
    dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state_info.pDynamicStates = dynamic_states.data();

    auto biding_description = Vertex::Get_binding_descriptor();
    auto attribute_description = Vertex::Get_attribute_descriptions();
    vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.pVertexBindingDescriptions = &biding_description;
    vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_description.size());
    vertex_input_info.pVertexAttributeDescriptions = attribute_description.data();

    vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.topology = vk::PrimitiveTopology::eTriangleList;

    vk::Viewport viewport{0.0f, 0.0f, static_cast<float>(swapchain_extent.width), static_cast<float>(swapchain_extent.height), 0.0f, 1.0f};
    vk::Rect2D scissor{vk::Offset2D{0, 0}, swapchain_extent};

    vk::PipelineViewportStateCreateInfo viewport_state{};
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.depthClampEnable = vk::False;
    rasterizer.rasterizerDiscardEnable = vk::False;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
}

vk::raii::ShaderModule eruptor::hardware::Pipeline::Create_shader_module(Device& device, const std::vector<char>& code) const
{
    vk::ShaderModuleCreateInfo create_info{};
    create_info.codeSize = code.size() * sizeof(char);
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    return vk::raii::ShaderModule{device.Get_device_handle(), create_info};
}

std::vector<char> eruptor::hardware::Pipeline::Read_file(const std::filesystem::path& path)
{
    std::ifstream file{path, std::ios::ate | std::ios::binary};

    if(!file.is_open())
    {
        throw std::runtime_error{"failed to open file!"};
    }

    std::vector<char> buffer(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

    file.close();
    return buffer;
}







