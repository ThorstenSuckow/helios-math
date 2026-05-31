# helios::math

Standalone math library for the helios module ecosystem.

## Overview

`helios::math` provides vector and matrix types, model/camera transform helpers,
projection utilities, and small math helpers used by rendering and runtime modules.

## Features

- `vec2`, `vec3`, `vec4` vector types
- `mat4` matrix type with column-major storage
- Axis-aligned bounding-box types
- Model transforms (`translate`, `rotate`, `scale`)
- Camera transforms (`perspective`, `orthographic`, `lookAt`)
- Angle helpers (`radians`, `degrees`)
- Numeric concepts for constrained templates

## Module surface

| Area | Public modules / APIs |
|------|------------------------|
| Types | `helios.math.types`, vector/matrix/AABB types |
| Transforms | `helios.math.transform`, model and camera transforms |
| Utilities | `helios.math.utils`, angle conversion helpers |
| Concepts | `helios.math.concepts`, numeric constraints |
| Tags | `helios.math.TransformType` |

## Usage

### C++ module

```cpp
import helios.math;
```

### Matrix conventions

All `mat4` matrices are stored in **column-major order**.

- `m[0..3]` = column 0
- `m[4..7]` = column 1
- `m[8..11]` = column 2
- `m[12..15]` = column 3

Element access follows row/column notation:

```cpp
T value = matrix(row, column); // internally: m[row + column * 4]
```

For a standard 4x4 transform matrix, translation is stored in column 3 and the
upper-left 3x3 block stores basis vectors.

`helios::math` uses column-vector convention:

```cpp
vec4f transformed = matrix * vec4f(point, 1.0f);
mat4f combined = B * A; // A first, then B.
```

### CMake

Build and install:

```bash
cmake -S . -B build -DHELIOS_MATH_ENABLE_PACKAGE=ON -DCMAKE_INSTALL_PREFIX="$PWD/install"
cmake --build build
cmake --install build
```

Consume from another project:

```cmake
find_package(helios-math CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE helios::math)
```

Configure a consumer against an installed prefix:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/helios-prefix"
cmake --build build
```

## Development

Run the regular CMake build from the repository root:

```bash
cmake -S . -B build
cmake --build build
```

## Related repositories

- [`helios-ecs`](https://github.com/thorstensuckow/helios-ecs)
- [`helios-engine`](https://github.com/thorstensuckow/helios-engine)
- [`helios-opengl`](https://github.com/thorstensuckow/helios-opengl)
- [`helios-glfw`](https://github.com/thorstensuckow/helios-glfw)
