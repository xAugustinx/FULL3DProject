cmake_minimum_required(VERSION 3.21) # HIP wymaga nowszego CMake (min. 3.21)
project(full3DProject C CXX HIP)
find_package(HIP REQUIRED)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(SOURCES copyMain.c)
find_package(SDL2 REQUIRED)
find_package(openGL)
find_package(SDL2_image REQUIRED)
add_executable(full3DProject ${SOURCES})

target_include_directories(full3DProject PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(full3DProject glfw vulkan ${SDL2_LIBRARIES} hip::host SDL2_image m)

