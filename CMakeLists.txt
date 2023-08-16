cmake_minimum_required(VERSION 3.6)
project(imgui-opengl-glfw-glew-cmake-demo)
#set(CMAKE_OSX_ARCHITECTURES "arm64")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(APP_TARGET 2d_engine)

add_compile_options(-g)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/engine)
add_executable(${APP_TARGET} src/main.cpp src/engine/render/render_init.cpp)

list(APPEND CMAKE_PREFIX_PATH "C:/Projects/2d_engine/deps/GLFW/lib/cmake/glfw3")
list(APPEND CMAKE_PREFIX_PATH "C:/glew-2.1.0/build/cmake/install/lib/cmake/glew")

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

if (WIN32)
# nothing now
endif (WIN32)

target_link_libraries(
        ${APP_TARGET}
        glfw
        ${OPENGL_LIBRARIES}
        GLEW::GLEW
        ${EXTRA_LIBS}
)