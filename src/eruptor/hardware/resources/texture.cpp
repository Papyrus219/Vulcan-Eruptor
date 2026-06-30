#include <Eruptor/lib/hardware/resources/texture.hpp>
#include <Eruptor/lib/hardware/device.hpp>

void eruptor::hardware::Texture::Init(Device & device, int width, int height, vk::Format format, vk::DeviceSize offset_in_stage_buffer)
{
    this->format = format;
    this->offset_in_stage_buffer = offset_in_stage_buffer;

    texture_image = device.Create_image(width, height, format, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
    texture_view = device.Create_image_view(texture_image, format, vk::ImageAspectFlagBits::eColor);
}
