#include <iostream>
#include <Eruptor/core.hpp>
#include <Eruptor/debug_handler.hpp>

int main()
{
    std::cout << "Hello, world!" << std::endl;

    eruptor::Core test_core{};
    test_core.Init();
    eruptor::Debug_handler deb_handler{};
    deb_handler.Init(test_core);

    test_core.Test();

    std::cout << "OOOO\n";

    return 0;
}
