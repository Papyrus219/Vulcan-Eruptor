#ifndef ERUPTOR_PLATFORM_PLATFORM_HPP
#define ERUPTOR_PLATFORM_PLATFORM_HPP

#include <Eruptor/lib/platform/core.hpp>
#include <Eruptor/lib/platform/device.hpp>

namespace eruptor::platform
{

class Platform
{
public:
    void Init();

    Core & Get_core() {return core;}
    Device & Get_device() {return device;}

private:
    Core core{};
    Device device{};
};

}

#endif // ERUPTOR_PLATFORM_PLATFORM_HPP
