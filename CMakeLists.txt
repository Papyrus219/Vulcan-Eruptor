cmake_minimum_required(VERSION 3.17)


project(ovum)

add_subdirectory(engine/)

include(cmake/modules/libs_maker.cmake)
include(cmake/modules/shader.cmake)

find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(APP_SRC src/main.cpp
src/app.cpp)

add_executable(app ${APP_SRC})
target_link_libraries(app PUBLIC eruptor_engine eruptor_event eruptor_scene)
target_include_directories(app PRIVATE include/)

install(TARGETS app RUNTIME DESTINATION bin)
