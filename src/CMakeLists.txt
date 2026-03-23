message(STATUS "including .h and .cpp files")

# Determine which backend platform folders to exclude
set(EXCLUDE_BACKENDS "android|linux|windows|macos|desktop") # Exclude all by default or if unknown
if(ANDROID)
    set(EXCLUDE_BACKENDS "linux|windows|macos|desktop")
elseif(LINUX)
    set(EXCLUDE_BACKENDS "android|windows|macos")
elseif(WINDOWS)
    set(EXCLUDE_BACKENDS "android|linux|macos")
elseif(MACOS)
    set(EXCLUDE_BACKENDS "android|linux|windows")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)


list(FILTER SOURCES EXCLUDE REGEX ".*/backends/(${EXCLUDE_BACKENDS})/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/old_in_render/.*")
target_sources(OGSEngine PRIVATE ${SOURCES})
target_include_directories(OGSEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/renderer/shaders)