cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0135 NEW)

project(eruptor)

include(FetchContent)
include(cmake/modules/libs_maker.cmake)
include(cmake/modules/shader.cmake)

find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

FetchContent_Declare(
        vmahpp
        URL      https://github.com/YaaZ/VulkanMemoryAllocator-Hpp/releases/download/v3.3.0%2B3/VulkanMemoryAllocator-Hpp-3.3.0.tar.gz
        URL_HASH SHA256=8c2a0573babe1f86f3241d12b4c9df40bb944649b7f4239c1c611797f8af6cd5
)
FetchContent_MakeAvailable(vmahpp)

set(HARDWARE_SRC src/eruptor/hardware.cpp
src/eruptor/hardware/core.cpp
src/eruptor/hardware/device.cpp
src/eruptor/hardware/window.cpp
src/eruptor/hardware/swapchain.cpp
src/eruptor/hardware/uniform_buffers.cpp
src/eruptor/hardware/pipeline.cpp
src/eruptor/hardware/command_manager.cpp
src/eruptor/hardware/vma.cpp
src/eruptor/hardware/utilities.cpp

src/eruptor/hardware/resources/resource_manager.cpp
src/eruptor/hardware/resources/texture.cpp
)

set(RESOURCE_SRC src/eruptor/resource/resource_manager.cpp
src/eruptor/resource/resource_handle.cpp
src/eruptor/resource/resource.cpp)

set(RENDERER_SRC src/eruptor/renderer/renderer.cpp
src/eruptor/renderer/cameras.cpp)

set(EVENT_SRC src/eruptor/event/event_manager.cpp
src/eruptor/event/event_listener.cpp)

set(ENGINE_SRC src/eruptor/eruptor.cpp)

add_slang_shader_target(main_shader SOURCES ${CMAKE_SOURCE_DIR}/shaders_src/shader.slang)

Make_library(TARGET eruptor_event
SOURCES EVENT_SRC
COMPILATION_FEATURES cxx_std_23)

Make_library(TARGET eruptor_hardware
SOURCES HARDWARE_SRC
LIBRARIES_TO_LINK_PUB Vulkan::Vulkan glm::glm glfw VulkanMemoryAllocator-Hpp::VulkanMemoryAllocator-Hpp
LIBRARIES_TO_LINK_PRIV eruptor_event
COMPILATION_FEATURES cxx_std_23)

add_dependencies(eruptor_hardware main_shader)
target_include_directories(eruptor_hardware PUBLIC ${vmahpp_SOURCE_DIR})
target_include_directories(eruptor_hardware PUBLIC ${vmahpp_SOURCE_DIR}/include)

Make_library(TARGET eruptor_resource
SOURCES RESOURCE_SRC
LIBRARIES_TO_LINK_PRIV eruptor_hardware assimp::assimp
LIBRARIES_TO_LINK_PRIV eruptor_event
COMPILATION_FEATURES cxx_std_23)

Make_library(TARGET eruptor_renderer
SOURCES RENDERER_SRC
LIBRARIES_TO_LINK_PRIV eruptor_hardware eruptor_resource Vulkan::Vulkan
LIBRARIES_TO_LINK_PRIV eruptor_event
COMPILATION_FEATURES cxx_std_23)

Make_library(TARGET eruptor_engine
SOURCES ENGINE_SRC
LIBRARIES_TO_LINK_PRIV eruptor_hardware eruptor_event
LIBRARIES_TO_LINK_PUB eruptor_resource eruptor_renderer
COMPILATION_FEATURES cxx_std_23)

add_executable(app src/main.cpp)
target_link_libraries(app PUBLIC eruptor_engine)
target_include_directories(app PRIVATE include/libs/)

install(TARGETS app RUNTIME DESTINATION bin)
