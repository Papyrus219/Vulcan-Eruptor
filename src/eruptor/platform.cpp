#include <Eruptor/lib/platform.hpp>

void eruptor::platform::Platform::Init()
{
    core.Init();
    device.Init( core );


}
