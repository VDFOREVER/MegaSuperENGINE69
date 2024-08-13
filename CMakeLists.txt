cmake_minimum_required(VERSION 3.10)

project(test)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_X11 OFF)
set(GLFW_BUILD_WAYLAND ON)

find_package(Bullet REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})
include_directories(include)
target_include_directories(${PROJECT_NAME} PRIVATE ${BULLET_INCLUDE_DIRS})

add_subdirectory(subprojects/glad)
add_subdirectory(subprojects/glfw)
add_subdirectory(subprojects/glm)
add_subdirectory(subprojects/tinygltf)
add_subdirectory(subprojects/soloud/contrib)

target_link_libraries(${PROJECT_NAME} glfw glad glm tinygltf soloud ${BULLET_LIBRARIES})

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
