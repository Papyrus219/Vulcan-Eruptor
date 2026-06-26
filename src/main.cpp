#include <iostream>
#include <vulkan/vulkan.h>
#include <Eruptor/lib/hardware.hpp>

int main()
{
    eruptor::hardware::Hardware test{};

    test.Init();

    return 0;
}
