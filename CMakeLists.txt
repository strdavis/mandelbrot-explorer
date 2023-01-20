# Require cmake version 3.2
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Initialize project.
project(cpp_project)

# Require C++ 17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Do not use legacy OpenGL.
set(OpenGL_GL_PREFERENCE GLVND)

# Obtain required packages.
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(fmt REQUIRED)
find_package(Catch2 3 REQUIRED)

# Create executables.
add_executable(mandelbrot_explorer app/mandelbrot_explorer.cpp app/mandelbrot_renderer.cpp app/text_renderer.cpp app/state_manager.cpp)
include_directories(include ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(mandelbrot_explorer ${OPENGL_opengl_LIBRARY} ${GLEW_LIBRARIES} glfw ${FREETYPE_LIBRARIES} fmt)
add_definitions(${GLEW_DEFINITIONS})

add_executable(test_state_manager test/test_state_manager.cpp app/state_manager.cpp)
target_link_libraries(test_state_manager PRIVATE Catch2::Catch2WithMain)