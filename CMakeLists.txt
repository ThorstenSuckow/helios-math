cmake_minimum_required(VERSION 4.0)
project(helios-math LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(CMAKE_IFC_OUTPUT_DIRECTORY)
    file(MAKE_DIRECTORY "${CMAKE_IFC_OUTPUT_DIRECTORY}")
endif()


file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE MODULE_INTERFACE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ixx")

list(FILTER CPP_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/build/")
list(FILTER MODULE_INTERFACE_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/build/")

add_library(helios_math STATIC)
add_library(helios::math ALIAS helios_math)
set_property(TARGET helios_math PROPERTY CXX_SCAN_FOR_MODULES ON)
target_compile_features(helios_math PUBLIC cxx_std_23)

target_sources(helios_math PRIVATE ${CPP_FILES})

if(MODULE_INTERFACE_FILES)
    target_sources(helios_math PUBLIC
            FILE_SET modules TYPE CXX_MODULES
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
            FILES ${MODULE_INTERFACE_FILES}
    )
endif()


target_include_directories(helios_math PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${HELIOS_MATH_GENERATED_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS helios_math
        EXPORT helios-mathTargets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        FILE_SET modules DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(FILES
        "${HELIOS_MATH_GENERATED_INCLUDE_DIR}/helios-engine-config.h"
        "${HELIOS_MATH_GENERATED_INCLUDE_DIR}/helios-math-config.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

set(HELIOS_MATH_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/helios-math")

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/helios-mathConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/helios-mathConfig.cmake"
        INSTALL_DESTINATION "${HELIOS_MATH_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/helios-mathConfigVersion.cmake"
        VERSION "0.1.0"
        COMPATIBILITY SameMajorVersion
)

install(EXPORT helios-mathTargets
        FILE helios-mathTargets.cmake
        NAMESPACE helios::
        DESTINATION "${HELIOS_MATH_INSTALL_CMAKEDIR}"
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/helios-mathConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/helios-mathConfigVersion.cmake"
        DESTINATION "${HELIOS_MATH_INSTALL_CMAKEDIR}"
)

if(BUILD_TESTING)
    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.17.0
    )
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
    )
    set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest glm)

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.test.cpp")

    add_executable(helios-math-tests ${TEST_SOURCES})
    target_link_libraries(helios-math-tests PRIVATE helios_math GTest::gtest_main glm::glm)

    include(GoogleTest)
    gtest_discover_tests(helios-math-tests)
endif()

option(BUILD_BENCHMARKS "Build benchmarks" ON)
if(BUILD_BENCHMARKS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/CMakeLists.txt")
    add_subdirectory(benchmarks)
endif()

