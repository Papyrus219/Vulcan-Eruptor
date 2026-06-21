#include <iostream>
#include <Eruptor/core.hpp>

int main()
{
    std::cout << "Hello, world!" << std::endl;

    eruptor::Core test{};
    test.Init();
    test.Test();

    std::cout << "OOOO\n";

    return 0;
}
