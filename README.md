# ZenithSTG-V

[日本語](README_ja.md) | **English**

Experimental bullet hell engine built with C++20, Vulkan, and SDL3.

**16,384 bullets on screen simultaneously — with barely a frame drop.** By offloading bullet position calculation and rendering entirely to the GPU through a Vulkan-native architecture, the engine keeps CPU overhead minimal while sustaining extreme bullet densities.

---

## Features

- **Extreme bullet density** — 16,384 simultaneous projectiles with negligible framerate impact
- **Vulkan-native** — Direct low-level Graphics API usage eliminates driver overhead
- **C++20** — Clean implementation leveraging modern language features like concepts and ranges
- **GLSL shaders** — Bullet rendering delegated to the GPU; large sprite batches processed efficiently
- **SDL3** — Cross-platform window management and input handling

---

## Dependencies

| Library | Purpose |
|---|---|
| Vulkan SDK | Graphics backend |
| SDL3 | Window management / input |
| GLM | Math library (vectors & matrices) |
| glslc (bundled with Vulkan SDK) | GLSL shader compilation |

---

## Building

### Requirements

- CMake 3.20 or higher
- C++20-capable compiler (GCC 12+ / Clang 14+ / MSVC 2022+)
- Vulkan SDK
- SDL3
- GLM

### Linux / macOS

```sh
# Compile shaders
./shader_compile.sh

# Build
./build.sh
```

Or manually:

```sh
cmake -B build
cmake --build build
```

### Windows (MSVC)

```sh
cmake -B build
cmake --build build --config Release
```

After building, `shaders/` and `textures/` are automatically copied next to the executable.

---

## Directory Structure

```
ZenithSTG-V/
├── src/          # C++ source code
├── include/      # Header files
├── shaders/      # GLSL shaders
├── textures/     # Texture assets
├── CMakeLists.txt
├── build.sh      # Build script
└── shader_compile.sh  # Shader compilation script
```

---

## License

MIT License — see [LICENSE](LICENSE) for details.
