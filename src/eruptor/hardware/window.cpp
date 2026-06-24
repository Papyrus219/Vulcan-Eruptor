#include <Eruptor/lib/hardware/window.hpp>
#include <Eruptor/lib/hardware/core.hpp>

void eruptor::hardware::Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void eruptor::hardware::Window::Create_window(std::string_view title, glm::uvec2 size)
{
    window = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
}

