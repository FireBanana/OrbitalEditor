cmake_minimum_required(VERSION 3.22)

include(FetchContent)
project(OrbitalEditor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =====================RETRIEVE ENGINE====================

# Pinned to an engine commit. To build against a local engine checkout instead:
#   cmake -B build -DFETCHCONTENT_SOURCE_DIR_ORBITAL=/path/to/Orbital
FetchContent_Declare(
    Orbital
    GIT_REPOSITORY https://github.com/FireBanana/Orbital.git
    GIT_TAG 7f0229c2d23a2dec6444f1a864e0b3bcc91508a7
)
FetchContent_MakeAvailable(Orbital)

# ========================================================

add_executable(OrbitalEditor
    src/main.cpp
    src/editor_application.h src/editor_application.cpp
    src/editor_context.h
    src/selection.h
    src/panels/panel.h
)

target_include_directories(OrbitalEditor PRIVATE src)

target_link_libraries(OrbitalEditor PRIVATE Orbital)
