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
│   │   ├── AppWindow.h         # Abstract window interface
│   │   ├── desktop/            # GLFW-based window for macOS/Windows/Linux
│   │   │   ├── window.h / .cpp
│   │   │   └── DesktopFileSystem.h / .cpp  # Desktop filesystem I/O
│   │   └── macos/              # macOS-specific code (.mm files)
│   │       ├── MetalWindowHelper.h / .mm
│   │       └── mactest.h / .cpp
│   ├── core/
│   │   ├── Timer.h / .cpp
│   │   └── Reference.h         # Base reference-counted type
│   ├── input/
│   │   ├── Input.h / .cpp
│   │   ├── InputProcessor.h / .cpp
│   │   ├── KeyboardCodes.h
│   │   ├── KeyboardController.h / .cpp
│   │   ├── MouseController.h / .cpp
│   │   └── TouchInputController.h / .cpp
│   ├── memory/
│   │   ├── Allocator.h / .cpp  # Singleton via getInstance()
│   │   ├── Pool.h / .cpp
│   │   ├── PoolManager.h / .cpp
│   │   └── memory.h
│   ├── monitoring/
│   │   ├── FPSCounter.h / .cpp
│   │   └── Logsystem.h
│   ├── concurrent/
│   │   ├── ThreadPoolExecutor.h
│   │   ├── Worker.h
│   │   ├── Task.h
│   │   └── Queue.h
│   ├── render/                 # Graphics abstraction layer
│   │   ├── IRenderer.h         # Pure interface: init/beginFrame/submit/endFrame
│   │   ├── RenderQueue.h       # RenderCommand + RenderQueue (opaque/transparent)
│   │   ├── RenderExtractor.h / .cpp  # Stateless: World → RenderQueue each frame
│   │   ├── RendererFactory.h / .cpp
│   │   ├── TextureHandle.h
│   │   ├── PixelFormat.h
│   │   └── metal/
│   │       ├── MetalRenderer.h / .cpp
│   │       ├── MetalImpl.cpp
│   │       └── shaders/triangle.metal
│   ├── storage/                # Asset management & resource system
│   │   ├── Resource.h          # Base class (extends Reference), enum ResType
│   │   ├── ResourceCache.h / .cpp
│   │   ├── ResourceHolder.h / .cpp
│   │   ├── AssetManager.h / .cpp   # Main API: genTexture/getAtlas/createMesh
│   │   ├── FileSystemUtil.h / .cpp # Platform-agnostic file I/O abstraction
│   │   ├── AtlasLoader.h / .cpp    # JSON-based texture atlas loading
│   │   ├── Texture.h / .cpp
│   │   ├── TextureAtlas.h / .cpp
│   │   ├── Mesh.h / .cpp
│   │   └── FreeTypeFont.h / .cpp
│   ├── world/                  # Scene graph
│   │   ├── GameObject.h        # Transform + BoundingSphere + mesh/material handles
│   │   ├── Camera.h            # Ortho/Perspective + Frustum (Gribb/Hartmann)
│   │   └── World.h / .cpp      # Owns objects list + Camera; spawn/destroy/update
│   ├── old_in_render/          # Legacy render code (to be migrated/removed)
│   │   ├── FreeTypeFont/Manager
│   │   ├── Mesh, Texture, TextureAtlas
│   │   └── SpriteBatch
│   └── utils/
├── res/                        # Runtime resources
│   ├── assets.json / assets.png
│   ├── buttons.pack / buttons.png
│   ├── enemiesArays.json
│   └── *.ttf / *.png
├── platforms/
│   └── macos/
│       └── main.cpp            # macOS entry point
├── external/
│   ├── glfw/                   # Windowing (desktop)
│   ├── glm/                    # Math
│   ├── freetype/               # Font rendering
│   ├── png/                    # PNG loading
│   ├── zlib/                   # Compression
│   └── json/                   # JSON parsing (nlohmann)
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

### OGSEngine fields (ownership order matters for destruction)

```cpp
FPSCounter           counter;
timer::game_timer    timer;
ThreadPoolExecutor*  executor;      // raw ptr — singleton
PoolManager*         poolManager;   // raw ptr — singleton
unique_ptr<AppWindow>    windowManager;
unique_ptr<IRenderer>    renderer;
unique_ptr<Input>        inputHandler;
unique_ptr<World>        world;
unique_ptr<AssetManager> assetManager;
RenderExtractor          extractor;  // stateless, value type
```

### Window: AppWindow

`ogs::AppWindow` — GLFW wrapper for desktop platforms.

```cpp
// REQUIRED when creating the window:
glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // do not create an OpenGL context
```

Lives as `std::unique_ptr<AppWindow>` in `OGSEngine`.

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

## Renderer Architecture

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

### Render pipeline (per frame)

```
World::update(dt)
      ↓
RenderExtractor::extract(world)   # frustum culling → RenderQueue
      ↓
IRenderer::beginFrame()
IRenderer::submit(queue)          # opaque (front-to-back), transparent (back-to-front)
IRenderer::endFrame()
```

### Layered architecture

```
Platform Layer    (Window, AppWindow)
      ↓
Graphics HAL      (IRenderer — pure interface)
      ↓
MetalRenderer / (VulkanRenderer — planned)
      ↓
RenderQueue       (opaque + transparent lists of RenderCommand)
      ↓
World / Scene     (GameObject, Camera)
      ↓
Game Logic
```

### RenderExtractor

Stateless object — extracts `RenderQueue` from `World` each frame.
Performs frustum culling using `Camera::getFrustum()` + `GameObject::bounds` (BoundingSphere).
`World` is the authority, `IRenderer` is the consumer.

### AssetManager

Central API for all resources. Communicates with `IRenderer` via callbacks:
```cpp
assetManager->setTextureCallbacks(
    [&](const TextureData& d) { return renderer->uploadTexture(d); },
    [&](TextureHandle h)      { renderer->releaseTexture(h); }
);
```

Resource types: `Texture`, `TextureAtlas`, `Mesh`, `FreeTypeFont` (extends `Resource` → `Reference`).
Atlas loading: JSON-based via `AtlasLoader` (reads `*.pack` / `assets.json`).

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
- `IRenderer` interface defined; `MetalRenderer` — partial implementation
- `AssetManager` — implemented (Texture, TextureAtlas, Mesh, async loading queue)
- `FileSystemUtil` — platform-agnostic I/O abstraction implemented
- `AtlasLoader` — JSON/pack-based atlas loading implemented
- `ResourceCache` / `ResourceHolder` — implemented
- `World` + `GameObject` + `Camera` — implemented (Ortho/Perspective, Frustum)
- `RenderExtractor` — implemented (frustum culling, builds RenderQueue)
- `RenderQueue` — opaque/transparent command lists with sorting hints
- `old_in_render/` — legacy SpriteBatch, FreeTypeManager, Mesh, Texture (to be migrated)
- ECS (EnTT) — not yet integrated
- Vulkan backend — not started
- Editor mainloop — skeleton exists, logic not connected

---

## Development Notes

- Add new systems as `std::unique_ptr` fields in `OGSEngine`, not as singletons
- `unique_ptr` destructors run in reverse declaration order — field order matters
- `.mm` files (Objective-C++) only for Metal-specific code
- On Android, the lifecycle (pause/resume) requires explicit swapchain recreation
- Check feature availability via `__cpp_lib_*` macros, not compiler version macros