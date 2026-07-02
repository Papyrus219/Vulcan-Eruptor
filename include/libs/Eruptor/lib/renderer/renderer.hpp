#ifndef ERUPTOR_RENDERER_RENDERER_HPP
#define ERUPTOR_RENDERER_RENDERER_HPP

#include <Eruptor/lib/resource/resource_manager.hpp>
#include <Eruptor/lib/resource/resource_handle.hpp>
#include <vector>

namespace eruptor
{
namespace hardware
{
class Hardware;
}

namespace renderer
{

struct Frame_sync;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Init(hardware::Hardware & hardware, resource::Resource_manager & resource_manager);

    bool Is_window_open();

    void Render_model(resource::Model_handle model_handle);

private:
    std::vector<Frame_sync> frame_syncs;

    hardware::Hardware * hardware{};
    resource::Resource_manager * rs_resource_manager{};

    uint32_t current_frame = 0;
};

}

}

#endif // ERUPTOR_RENDERER_RENDERER_HPP
