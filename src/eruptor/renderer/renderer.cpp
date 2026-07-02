#include <Eruptor/lib/renderer/renderer.hpp>
#include <Eruptor/lib/hardware.hpp>
#include <Eruptor/lib/hardware/utilities.hpp>
#include <Eruptor/lib/resource/resource_manager.hpp>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <iostream>

struct eruptor::renderer::Frame_sync
{
    vk::raii::Semaphore present_complete = nullptr;
    vk::raii::Semaphore render_finished = nullptr;
    vk::raii::Fence in_flight_fences = nullptr;
};

eruptor::renderer::Renderer::Renderer()
{

}

void eruptor::renderer::Renderer::Init(hardware::Hardware & hardware, resource::Resource_manager & resource_manager)
{
    this->hardware = &hardware;
    this->rs_resource_manager = &resource_manager;

    auto & device = this->hardware->Get_device();

    vk::FenceCreateInfo fence_info{};
    fence_info.flags = vk::FenceCreateFlagBits::eSignaled;

    uint32_t image_count = this->hardware->Get_swapchain().Get_image_count(); // patrz punkt 3 niżej

    for(uint32_t i{}; i < image_count; i++)
    {
        frame_syncs.push_back({});
        frame_syncs.back().present_complete = vk::raii::Semaphore{device.Get_device_handle(), vk::SemaphoreCreateInfo{}};
        frame_syncs.back().render_finished = vk::raii::Semaphore{device.Get_device_handle(), vk::SemaphoreCreateInfo{}};
        frame_syncs.back().in_flight_fences = vk::raii::Fence{device.Get_device_handle(), fence_info};
    }
}

void eruptor::renderer::Renderer::Render_model(resource::Model_handle model_handle)
{

    auto & device = hardware->Get_device();
    auto & swap_chain = hardware->Get_swapchain();
    auto & hw_resource_manager = hardware->Get_resource_manager();
    auto & command_manager = hardware->Get_command_manager();
    auto & command_buffor = command_manager.Begin_graphic_command_record(0);

    auto fenceResult = device.Get_device_handle().waitForFences(*frame_syncs[current_frame].in_flight_fences, vk::True, UINT64_MAX);
    if (fenceResult != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to wait for fence!");
    }
    device.Get_device_handle().resetFences(*frame_syncs[current_frame].in_flight_fences);

    auto [result, imageIndex] = swap_chain.Get_swapchain_handle().acquireNextImage(UINT64_MAX, *frame_syncs[current_frame].present_complete, nullptr);

    hardware::utilities::Transition_image_layout(
        command_buffor,
        swap_chain.Get_image(imageIndex),
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::QueueFamilyIgnored,
        vk::QueueFamilyIgnored,
        vk::ImageAspectFlagBits::eColor
    );
    vk::ClearValue              clearColor     = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
    vk::RenderingAttachmentInfo attachmentInfo = {};
    attachmentInfo.setImageView(swap_chain.Get_image_view(imageIndex));
    attachmentInfo.setImageLayout(vk::ImageLayout::eColorAttachmentOptimal);
    attachmentInfo.setLoadOp(vk::AttachmentLoadOp::eClear);
    attachmentInfo.setStoreOp(vk::AttachmentStoreOp::eStore);
    attachmentInfo.setClearValue( clearColor );

    vk::Rect2D render_area{};
    render_area.setOffset({0,0});
    render_area.setExtent( swap_chain.Get_extent() );

    vk::RenderingInfo renderingInfo = {};
    renderingInfo.setRenderArea( render_area );
    renderingInfo.setLayerCount(1);
    renderingInfo.setColorAttachments( attachmentInfo );

    command_buffor.beginRendering(renderingInfo);
    command_buffor.bindPipeline(vk::PipelineBindPoint::eGraphics, hardware->Get_pipeline().Get_pipeline_handle());
    command_buffor.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(swap_chain.Get_extent().width), static_cast<float>(swap_chain.Get_extent().height), 0.0f, 1.0f));
    command_buffor.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), swap_chain.Get_extent()));

    hw_resource_manager.Bind_geometry_buffer( command_buffor );

    auto & model = rs_resource_manager->Get_model( model_handle );

    for(auto mesh_handle : model.Meshes_handles)
    {
        auto mesh = hw_resource_manager.Get_mesh( mesh_handle );

        command_buffor.drawIndexed(mesh.indices_amount, 1, mesh.indices_offset, mesh.vertex_offset, 0);
    }

    command_buffor.endRendering();

            // After rendering, transition the swapchain image to vk::ImageLayout::ePresentSrcKHR
    hardware::utilities::Transition_image_layout(
        command_buffor,
        swap_chain.Get_image(imageIndex),
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::ImageLayout::ePresentSrcKHR,
        vk::QueueFamilyIgnored,                // srcAccessMask
        vk::QueueFamilyIgnored,                                     // dstAccessMask
        vk::ImageAspectFlagBits::eColor                  // dstStage
    );

    command_buffor.end();
    command_manager.Submit_graphic_commands(command_buffor, std::vector<vk::PipelineStageFlags>{vk::PipelineStageFlagBits::eColorAttachmentOutput}, std::vector{*frame_syncs[current_frame].present_complete}, std::vector{*frame_syncs[current_frame].render_finished}, *frame_syncs[current_frame].in_flight_fences);

    device.queues.Get_graphics_queue_handle().waitIdle();

        vk::PresentInfoKHR presentInfoKHR{};
        presentInfoKHR.setWaitSemaphores( *frame_syncs[current_frame].render_finished );
        presentInfoKHR.setSwapchains( *swap_chain.Get_swapchain_handle() );
        presentInfoKHR.setImageIndices( imageIndex );

        result = device.queues.Get_graphics_queue_handle().presentKHR(presentInfoKHR);
        switch (result)
        {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eSuboptimalKHR:
                break;
            default:
                break;        // an unexpected result is returned!
        }

    current_frame = (current_frame + 1) % static_cast<uint32_t>(frame_syncs.size());
}

bool eruptor::renderer::Renderer::Is_window_open()
{
    glfwPollEvents();
    return !glfwWindowShouldClose(hardware->Get_window().Get_glfw_window());
}

eruptor::renderer::Renderer::~Renderer()
{

}
