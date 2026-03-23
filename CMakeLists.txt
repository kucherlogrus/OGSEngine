cmake_minimum_required(VERSION 4.1)

project(OGSEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Current platform: ${CMAKE_SYSTEM_NAME}")

set(PLATFORM_FOUND false)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DMACOS)
    set(MACOS true)
    set(PLATFORM_FOUND true)
endif ()

if (PLATFORM_FOUND)
    message(STATUS "Platform definitions set successfully.")
else ()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif ()

include_directories(
    src
    external
    external/glfw/include/GLFW
    external/glm
    external/freetype/include
    external/png
    external/json/src
    ${CMAKE_BINARY_DIR}/external/png
)

if(MACOS)
    include_directories(external/metal-cpp)
endif()

add_subdirectory(external)
add_library(OGSEngine STATIC)
add_subdirectory(src)

if(MACOS)
    include_directories(platforms/macos)

    # Compile Metal shaders into a .metallib and embed into the app bundle
    set(METAL_SHADER src/render/metal/shaders/triangle.metal)
    set(METALLIB_OUTPUT ${CMAKE_BINARY_DIR}/default.metallib)

    add_custom_command(
        OUTPUT ${METALLIB_OUTPUT}
        COMMAND xcrun -sdk macosx metal -c ${CMAKE_SOURCE_DIR}/${METAL_SHADER} -o ${CMAKE_BINARY_DIR}/triangle.air
        COMMAND xcrun -sdk macosx metallib ${CMAKE_BINARY_DIR}/triangle.air -o ${METALLIB_OUTPUT}
        DEPENDS ${CMAKE_SOURCE_DIR}/${METAL_SHADER}
        COMMENT "Compiling Metal shaders"
    )
    add_custom_target(MetalShaders DEPENDS ${METALLIB_OUTPUT})

    add_executable(game platforms/macos/main.cpp)
    add_dependencies(game MetalShaders)
    target_link_libraries(game OGSEngine glfw freetype png_static
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework Metal"
        "-framework QuartzCore"
    )

    # Copy metallib next to the executable so newDefaultLibrary() finds it
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${METALLIB_OUTPUT} $<TARGET_FILE_DIR:game>/default.metallib
        COMMENT "Copying default.metallib to output directory"
    )
endif()

