cmake_minimum_required(VERSION 3.10)
project(idetest)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} main.c os.c render.c)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)