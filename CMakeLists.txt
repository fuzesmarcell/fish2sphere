cmake_minimum_required(VERSION 3.20)

project(Fish2Sphere VERSION 1.0
                    DESCRIPTION "Fisheye to spherical"
                    LANGUAGES CXX)

option(F2S_TOOLS "Include simple tools" TRUE)

if (F2S_TOOLS)
    include(FetchContent)

    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb
    )

    FetchContent_MakeAvailable(stb)

    message(${stb_SOURCE_DIR})

    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

endif()

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_library(fish2sphere_lib
    "fish2sphere.h"
    "fish2sphere.cpp")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(fish2sphere_lib PUBLIC $<$<CONFIG:Release>:OpenMP::OpenMP_CXX>)
endif()

add_executable(fish2sphere
    "main.cpp"
    "stb.cpp")

target_link_libraries(fish2sphere
    fish2sphere_lib
    stb)