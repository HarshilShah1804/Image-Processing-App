cmake_minimum_required(VERSION 3.10)

project(ImageNodeApp)

# Suppress OpenGL policy warning
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW via pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/imgui
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends
    ${CMAKE_SOURCE_DIR}/lib/imgui-node-editor
    ${CMAKE_SOURCE_DIR}/lib/imgui-node-editor/examples/application/source
    ${CMAKE_SOURCE_DIR}/lib/imnodes
    ${CMAKE_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

# Gather source files
file(GLOB IMGUI_SOURCES
    lib/imgui/*.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

file(GLOB NODE_EDITOR_SOURCES
    lib/imgui-node-editor/*.cpp
)

file(GLOB IMNODES_SOURCES
    lib/imnodes/*.cpp
)

file(GLOB APP_SOURCES
    src/**/*.cpp
    app.cpp
)

add_executable(ImageNodeApp
    ${IMGUI_SOURCES}
    ${IMNODES_SOURCES}
    ${NODE_EDITOR_SOURCES}
    ${APP_SOURCES}
)

# Link everything
target_link_libraries(ImageNodeApp
    ${OpenCV_LIBS}
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    glfw
    GL
    dl
    pthread
)
