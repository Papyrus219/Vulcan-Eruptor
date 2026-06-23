cmake_minimum_required(VERSION 3.10)

project(eruptor)
include(cmake/modules/libs_maker.cmake)

function (add_slang_shader_target TARGET)
  cmake_parse_arguments ("SHADER" "" "" "SOURCES" ${ARGN})
  set (SHADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders)
  set (ENTRY_POINTS -entry vertMain -entry fragMain)
  add_custom_command (
          OUTPUT ${SHADERS_DIR}
          COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
  )
  add_custom_command (
          OUTPUT  ${SHADERS_DIR}/slang.spv
          COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
          WORKING_DIRECTORY ${SHADERS_DIR}
          DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
          COMMENT "Compiling Slang Shaders"
          VERBATIM
  )
  add_custom_target (${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
endfunction()

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(PLATFORM_SRC src/eruptor/platform.cpp
src/eruptor/platform/core.cpp
src/eruptor/platform/device.cpp
src/eruptor/platform/window.cpp
src/eruptor/platform/swapchain.cpp)

Make_library(TARGET eruptor
SOURCES PLATFORM_SRC
LIBRARIES_TO_LINK_PUB Vulkan::Vulkan glm::glm glfw
COMPILATION_FEATURES cxx_std_23)

add_executable(app src/main.cpp)
target_link_libraries(app PUBLIC eruptor)

install(TARGETS app RUNTIME DESTINATION bin)
