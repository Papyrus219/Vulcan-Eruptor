#include <iostream>
#include <app.hpp>


int main()
{
    ovum::App app{};
    app.Init();
    app.Start_loop();

    return 0;
}
