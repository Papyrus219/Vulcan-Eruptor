cmake_minimum_required(VERSION 3.17)
project(ovum)

add_subdirectory(engine/)

set(APP_SRC ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/app.cpp)

add_executable(app ${APP_SRC})
target_link_libraries(app PUBLIC eruptor_engine eruptor_event eruptor_scene eruptor_physic)
target_include_directories(app PRIVATE include/)
target_compile_features(app PRIVATE cxx_std_23)

install(TARGETS app RUNTIME DESTINATION bin)
