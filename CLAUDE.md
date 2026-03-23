# OGSEngine — Game Engine

## Language Policy

**All code, comments, identifiers, commit messages, and documentation must be written in English.**
This applies to: source files, headers, CMake files, inline comments, log messages, and any text generated during development.

---

## Project Overview

Cross-platform game engine in C++20 with multiple graphics backends.

**Target platforms:**
| Platform | Graphics Backend    |
|----------|---------------------|
| macOS    | Metal (native)      |
| iOS      | Metal (native)      |
| Windows  | Vulkan              |
| Linux    | Vulkan              |
| Android  | Vulkan (NDK r25+)   |

Two backends total: **Metal** (Apple) and **Vulkan** (everything else).

---

## Project Structure

```
OGSEngine/
├── CMakeLists.txt              # Root CMake
├── src/                        # Engine sources → compiled into libOGSEngine.a
│   ├── CMakeLists.txt          # GLOB_RECURSE with platform backend filtering
│   ├── OGSEngine.h / .cpp      # Central engine class
│   ├── backends/
│   │   ├── desktop/            # GLFW-based window for macOS/Windows/Linux
│   │   │   ├── window.h
│   │   │   └── window.cpp
│   │   └── macos/              # macOS-specific code (.mm files)
│   ├── core/
│   │   └── Timer.h / .cpp
│   ├── input/
│   │   ├── Input.h / .cpp
│   │   ├── KeyboardController.h / .cpp
│   │   ├── MouseController.h / .cpp
│   │   └── TouchInputController.h / .cpp
│   ├── memory/
│   │   ├── Allocator.h / .cpp  # Singleton via getInstance()
│   │   ├── Pool.h / .cpp
│   │   └── PoolManager.h / .cpp
│   ├── monitoring/
│   │   ├── FPSCounter.h / .cpp
│   │   └── Logsystem.h
│   ├── concurrent/
│   │   ├── ThreadPoolExecutor.h
│   │   ├── Worker.h
│   │   ├── Task.h
│   │   └── Queue.h
│   └── utils/
├── platforms/
│   └── macos/
│       └── main.cpp            # macOS entry point
├── external/
│   ├── glfw/                   # Windowing (desktop)
│   ├── glm/                    # Math
│   ├── freetype/               # Font rendering
│   ├── png/                    # PNG loading
│   ├── zlib/                   # Compression
│   └── json/                   # JSON parsing
└── cmake-build-debug/
```

---

## Language Standard and Compilers

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)   # no GNU extensions — critical for cross-platform
```

| Platform    | Compiler        | STL                  |
|-------------|-----------------|----------------------|
| macOS/iOS   | Apple Clang     | libc++               |
| Windows     | MSVC / Clang-cl | MSVC STL             |
| Android     | Clang (NDK)     | libc++               |
| Linux       | GCC / Clang     | libstdc++ / libc++   |

**Do not use C++ Modules (`import`)** — toolchain support is still unstable across all platforms as of 2026.

Use feature-test macros (`__cpp_lib_format`) to check feature availability, not compiler version macros.

---

## Build System

Root `CMakeLists.txt` detects the platform via `CMAKE_SYSTEM_NAME` and sets defines (`-DMACOS`, `-DANDROID`, etc.).

`src/CMakeLists.txt` collects sources via `GLOB_RECURSE` and filters out irrelevant platform backends:

```cmake
list(FILTER SOURCES EXCLUDE REGEX ".*/backends/(${EXCLUDE_BACKENDS})/.*")
```

Final linking in root CMake:

```cmake
add_library(OGSEngine STATIC)   # → libOGSEngine.a
add_executable(game platforms/macos/main.cpp)
target_link_libraries(game OGSEngine glfw freetype png_static
    "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
```

---

## Engine Architecture

### Central class: OGSEngine

`OGSEngine` owns all subsystems and controls initialization/destruction order.

```
main.cpp
  └── OGSEngine::OGSEngine()       # constructor
  └── OGSEngine::initCoreSystems() # subsystem initialization
  └── OGSEngine::requestloop()     # main loop
        ├── gameMainloop()         # AppTYPE::GAME
        └── editorMainloop()       # AppTYPE::EDITOR
```

**States:** `AppState` (RUN / PAUSE / CLOSE), `AppTYPE` (EDITOR / GAME).

### Initialization order in `initCoreSystems()`

1. `Allocator::getInstance()` — custom memory allocator
2. `PoolManager::getInstance()` — memory pool
3. `ThreadPoolExecutor(2)` — thread pool
4. `Input::getInstance()` — input system

### Window: AppWindow

`ogs::AppWindow` — GLFW wrapper for desktop platforms.

```cpp
// REQUIRED when creating the window:
glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // do not create an OpenGL context
```

Lives as a value field in `OGSEngine` (not a pointer).

**GLFW callback pattern:**
```cpp
glfwSetWindowUserPointer(window, this);
glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
    auto* self = static_cast<MyClass*>(glfwGetWindowUserPointer(w));
    self->onKey(key, scancode, action, mods);
});
```
If multiple pointers need to be stored, wrap them in a `WindowContext` struct.

---

## Planned Renderer Architecture

### Two graphics backends

**Metal (macOS/iOS):**
GLFW has no native Metal support. Use `glfwGetCocoaWindow()` to get `NSWindow*`, then create `CAMetalLayer`. This code lives in `.mm` files (Objective-C++).

**Vulkan (Windows/Linux/Android):**
GLFW supports Vulkan natively:
```cpp
glfwCreateWindowSurface(instance, window, nullptr, &surface);
```
On Android, use `ANativeWindow` from the NDK instead of GLFW.

### Platform abstraction

Two distinct layers:

```
Layer 1: Window  (platform container)
         DesktopWindow (GLFW) / AndroidWindow (ANativeWindow)
         ↓
Layer 2: RenderSurface  (graphics backend)
         VulkanSurface (VkSurfaceKHR) / MetalSurface (CAMetalLayer)
```

### Layered architecture

```
Platform Layer    (Window, AppWindow)
      ↓
Graphics HAL      (IDevice, ICommandBuffer — pure interfaces)
      ↓
VulkanBackend / MetalBackend
      ↓
Render Graph      (declarative passes, dependency tracking)
      ↓
Scene / ECS       (EnTT planned)
      ↓
Game Logic
```

---

## System Management

### Current state (partial singletons)

`Allocator`, `PoolManager`, `Input` use `getInstance()`. This is a temporary solution.

### Target pattern: Engine as owner + EngineContext

Systems should not pull dependencies via singletons — they receive them via constructor or `EngineContext&`.

The logger is an acceptable singleton (needed before everything else initializes).

### Inter-system communication

Use an `EventBus` (type-based subscribe/emit) to avoid circular dependencies between systems.

---

## External Dependencies

| Library  | Purpose                                |
|----------|----------------------------------------|
| GLFW     | Windows, input, Vulkan surface (desktop) |
| GLM      | Math (vectors, matrices)               |
| FreeType | Font rendering                         |
| libpng   | PNG loading                            |
| zlib     | Compression                            |
| json     | JSON parsing (nlohmann or similar)     |

**Planned:** VMA (Vulkan Memory Allocator), Dear ImGui, cgltf/stb_image, EnTT (ECS).

**Shaders:** GLSL → SPIR-V (Vulkan), MSL (Metal native), spirv-cross for cross-compilation.

---

## Current Status

- Basic engine skeleton builds on macOS (arm64)
- `AppWindow` with GLFW works (constructor, destructor, `createWindow`)
- Input, Memory, ThreadPool — basic implementations in place
- Renderer, AssetManager, Scene, ECS — commented out, planned
- Editor/Game mainloop — skeleton exists, logic not connected

---

## Development Notes

- Add new systems as `std::unique_ptr` fields in `OGSEngine`, not as singletons
- `unique_ptr` destructors run in reverse declaration order — field order matters
- `.mm` files (Objective-C++) only for Metal-specific code
- On Android, the lifecycle (pause/resume) requires explicit swapchain recreation
- Check feature availability via `__cpp_lib_*` macros, not compiler version macros