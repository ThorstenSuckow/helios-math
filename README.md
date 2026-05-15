# helios::math

This namespace provides comprehensive mathematical functionality including vector and matrix types, transformation utilities, mathematical concepts, and utility functions for 3D graphics and game framework computations. Provides all mathematical operations required for 3D graphics, including transformations, projections, and spatial computations.

## CMake package usage

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



---
<details>
<summary>Doxygen</summary><p>
@namespace helios::math
@brief Mathematical operations and types.
@details This namespace provides comprehensive mathematical functionality including vector and matrix types, transformation utilities, mathematical concepts, and utility functions for 3D graphics and game framework computations. Provides all mathematical operations required for 3D graphics, including transformations, projections, and spatial computations.
</p></details>
