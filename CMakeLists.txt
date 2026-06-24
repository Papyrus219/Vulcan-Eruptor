cmake_minimum_required(VERSION 3.10)

project(eruptor)
include(cmake/modules/libs_maker.cmake)
include(cmake/modules/shader.cmake)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(HARDWARE_SRC src/eruptor/hardware.cpp
src/eruptor/hardware/core.cpp
src/eruptor/hardware/device.cpp
src/eruptor/hardware/window.cpp
src/eruptor/hardware/swapchain.cpp
src/eruptor/hardware/pipeline.cpp)

set(RESOURCE_SRC src/eruptor/resource/resource_manager.cpp
src/eruptor/resource/resource_handle.cpp
src/eruptor/resource/resource.cpp)

add_slang_shader_target(main_shader SOURCES ${CMAKE_SOURCE_DIR}/shaders_src/shader.slang)

Make_library(TARGET eruptor_hardware
SOURCES HARDWARE_SRC
LIBRARIES_TO_LINK_PRIV Vulkan::Vulkan glm::glm glfw
COMPILATION_FEATURES cxx_std_23)

add_dependencies(eruptor_hardware main_shader)

Make_library(TARGET eruptor_resource
SOURCES RESOURCE_SRC
LIBRARIES_TO_LINK_PRIV eruptor_hardware
COMPILATION_FEATURES cxx_std_23)

add_executable(app src/main.cpp)
target_link_libraries(app PUBLIC eruptor_resource)

install(TARGETS app RUNTIME DESTINATION bin)
