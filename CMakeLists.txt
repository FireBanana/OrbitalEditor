cmake_minimum_required(VERSION 3.22)

project(OrbitalEditor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =====================RETRIEVE ENGINE====================

# The engine is built from a local checkout next to the editor (../Orbital).
set(ORBITAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Orbital")
if(NOT EXISTS "${ORBITAL_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "Orbital engine not found at ${ORBITAL_DIR}")
endif()
add_subdirectory("${ORBITAL_DIR}" "${CMAKE_BINARY_DIR}/orbital")

# ========================================================

add_executable(OrbitalEditor
    src/main.cpp
    src/editor_application.h src/editor_application.cpp
    src/editor_context.h
    src/selection.h
    src/panels/panel.h
    src/panels/dockspace_layout.h src/panels/dockspace_layout.cpp
    src/panels/stats_panel.h src/panels/stats_panel.cpp
)

target_include_directories(OrbitalEditor PRIVATE src)

target_link_libraries(OrbitalEditor PRIVATE Orbital)
