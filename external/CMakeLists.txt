message(STATUS "external libs")

### glm ###
add_subdirectory(glm)

### png ###
set(PNG_SHARED FALSE)
set(PNG_STATIC TRUE)
set(PNG_TESTS FALSE)
set(PNG_TOOLS OFF)
add_subdirectory(png)

message(STATUS "LibPNG package was found with include dir: '${CMAKE_CURRENT_SOURCE_DIR}/png'")

### json ###
include_directories(json)

### freetype ###
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)
set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE)
add_subdirectory(freetype)


### GLFW ###
add_subdirectory (glfw)

include_directories(
	glfw/include/GLFW/
	glew/include/
)





