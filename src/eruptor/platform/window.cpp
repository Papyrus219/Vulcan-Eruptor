#include <Eruptor/lib/platform/window.hpp>
#include <Eruptor/lib/platform/core.hpp>

void eruptor::platform::Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void eruptor::platform::Window::Create_window(std::string_view title, glm::uvec2 size)
{
    window = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
}

