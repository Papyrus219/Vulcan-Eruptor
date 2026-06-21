include_guard()

option(PAP_CMAKE_SILENCE_MAKE_LIBRARY OFF "Turn off  Make_library debug messages.")
option(PAP_CMAKE_SILENCE_MAKE_HEADER_LIBRARY OFF "Turn off  Make_header_library debug messages.")

function(Make_library)
    set(no_values HAS_CUSTOM_INCLUDE_PATH)
    set(single_values TARGET SOURCES CUSTOM_INCLUDE_PATH)
    set(multi_values COMPILATION_FEATURES LIBRARIES_TO_LINK_PUB LIBRARIES_TO_LINK_PRIV)

    cmake_parse_arguments(
        arg
        "${no_values}" "${single_values}" "${multi_values}"
        ${ARGN})

    if(NOT PAP_CMAKE_SILENCE_MAKE_LIBRARY)
        message("Setup ${arg_TARGET} library: Start")
    endif()

    add_library(${arg_TARGET} ${${arg_SOURCES}})

    if(arg_HAS_CUSTOM_INCLUDE_PATH)
        target_include_directories(${arg_TARGET} PUBLIC arg_CUSTOM_INCLUDE_PATH)
    else()
        target_include_directories(${arg_TARGET} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/libs/${arg_TARGET}")
    endif()

    target_compile_features(${arg_TARGET} PRIVATE ${arg_COMPILATION_FEATURES})
    target_link_libraries(${arg_TARGET}
    PUBLIC ${arg_LIBRARIES_TO_LINK_PUB}
    PRIVATE ${arg_LIBRARIES_TO_LINK_PRIV})

    if(NOT PAP_CMAKE_SILENCE_MAKE_LIBRARY)
        message("Setup ${arg_TARGET} library: Done")
    endif()
endfunction()

function(Make_header_library)
    set(no_values HAS_CUSTOM_INCLUDE_PATH)
    set(single_values TARGET CUSTOM_INCLUDE_PATH)
    set(multi_values COMPILATION_FEATURES LIBRARIES_TO_LINK)

    cmake_parse_arguments(
        arg
        "${no_values}" "${single_values}" "${multi_values}"
        ${ARGN})

    if(NOT PAP_CMAKE_SILENCE_MAKE_HEADER_LIBRARY)
        message("Setup ${arg_TARGET} library: Start")
    endif()

    add_library(${arg_TARGET} INTERFACE)

    if(arg_HAS_CUSTOM_INCLUDE_PATH)
        target_include_directories(${arg_TARGET} INTERFACE arg_CUSTOM_INCLUDE_PATH)
    else()
        target_include_directories(${arg_TARGET} INTERFACE "include/libs/${arg_TARGET}")
    endif()

    target_compile_features(${arg_TARGET} INTERFACE cxx_std_23)
    target_link_libraries(${arg_TARGET} INTERFACE ${LIBRARIES_TO_LINK})

    if(NOT PAP_CMAKE_SILENCE_MAKE_HEADER_LIBRARY)
        message("Setup ${arg_TARGET} library: Done.")
    endif()

endfunction()

