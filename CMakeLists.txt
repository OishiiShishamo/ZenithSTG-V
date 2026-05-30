cmake_minimum_required(VERSION 3.20)

project(zenith-stg-v LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(zenith-stg-v ${SRC})

target_include_directories(zenith-stg-v PRIVATE include)

target_link_libraries(zenith-stg-v PRIVATE
    Vulkan::Vulkan
    SDL3::SDL3
    glm::glm
)

if(MSVC)

    target_compile_options(zenith-stg-v PRIVATE
        /W4
        /permissive-
    )

else()

    target_compile_options(zenith-stg-v PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()
