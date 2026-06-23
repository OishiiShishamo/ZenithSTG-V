cmake_minimum_required(VERSION 3.20)

project(zenith-stg-v LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)
find_package(TBB REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    src/*.cpp
)

if(WIN32)
    add_executable(zenith-stg-v WIN32 ${SRC})
else()
    add_executable(zenith-stg-v ${SRC})
endif()

target_include_directories(zenith-stg-v PRIVATE include)

target_link_libraries(zenith-stg-v PRIVATE
    Vulkan::Vulkan
    SDL3::SDL3
    glm::glm
    TBB::tbb
)

if(WIN32)
    target_link_libraries(zenith-stg-v PRIVATE SDL3::SDL3_main)
endif()

if(MSVC)

    target_compile_options(zenith-stg-v PRIVATE
        /W4
        /permissive-
    )
    target_compile_definitions(zenith-stg-v PRIVATE
        VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
        _CRT_SECURE_NO_WARNINGS
    )

else()

    target_compile_options(zenith-stg-v PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    target_compile_definitions(zenith-stg-v PRIVATE
        VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
    )

endif()

add_custom_command(TARGET zenith-stg-v POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:zenith-stg-v>/shaders
)

add_custom_command(TARGET zenith-stg-v POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/textures
    $<TARGET_FILE_DIR:zenith-stg-v>/textures
)

add_custom_command(TARGET zenith-stg-v POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/sounds
    $<TARGET_FILE_DIR:zenith-stg-v>/sounds
)

add_custom_command(TARGET zenith-stg-v POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts
    $<TARGET_FILE_DIR:zenith-stg-v>/fonts
)

if(WIN32)
    add_custom_command(TARGET zenith-stg-v POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:zenith-stg-v>
    )
    add_custom_command(TARGET zenith-stg-v POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:TBB::tbb>
        $<TARGET_FILE_DIR:zenith-stg-v>
    )
endif()
