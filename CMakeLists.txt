cmake_minimum_required(VERSION 3.11)

project(FirstRenderingEngine)

find_package(GLEW required)
find_package(glfw3 CONFIG REQUIRED)

add_executable(first_rendering_engine src/main.cpp)
set_target_properties(first_rendering_engine PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)
target_include_directories(first_rendering_engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/include")
target_link_libraries(first_rendering_engine PRIVATE GLEW::GLEW glfw)