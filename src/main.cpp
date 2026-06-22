#include <iostream>
#include <Eruptor/lib/platform.hpp>

int main()
{
    eruptor::platform::Platform test{};
    test.Init();

    return 0;
}
