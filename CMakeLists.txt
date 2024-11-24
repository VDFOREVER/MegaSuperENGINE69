cmake_minimum_required(VERSION 3.10)
project(ms_engine69)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

find_package(Bullet REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND wayland-client)
pkg_check_modules(X11 x11)

if(WAYLAND_FOUND)
    message(STATUS "Wayland detected")
	set(GLFW_BUILD_X11 OFF)
	set(GLFW_BUILD_WAYLAND ON)
	set(BUILD_TYPE_SUFFIX "wayland")
elseif(X11_FOUND)
    message(STATUS "X11 detected")
	set(GLFW_BUILD_X11 ON)
	set(GLFW_BUILD_WAYLAND OFF)
	set(BUILD_TYPE_SUFFIX "x11")
else()
    message(FATAL_ERROR "Neither Wayland nor X11 detected")
endif()

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_${BUILD_TYPE_SUFFIX}")
include_directories(include)
target_include_directories(${PROJECT_NAME} PRIVATE ${BULLET_INCLUDE_DIRS})

add_subdirectory(subprojects/glad)
add_subdirectory(subprojects/glfw)
add_subdirectory(subprojects/glm)
add_subdirectory(subprojects/tinygltf)

SET(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)

target_link_libraries(${PROJECT_NAME} glfw glad glm tinygltf ${BULLET_LIBRARIES})

#imgui
set(IMGUI_SOURCES
	subprojects/imgui/imgui.cpp
	subprojects/imgui/backends/imgui_impl_glfw.cpp
	subprojects/imgui/backends/imgui_impl_opengl3.cpp
	subprojects/imgui/imgui_demo.cpp
	subprojects/imgui/imgui_tables.cpp
	subprojects/imgui/imgui_widgets.cpp
	subprojects/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC subprojects)
target_include_directories(ImGui PUBLIC subprojects/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${PROJECT_NAME} ImGui)

if (RESOURCE_PATH)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DRESOURCE_PATH="${RESOURCE_PATH}")
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DRESOURCE_PATH="../resource/")
endif()