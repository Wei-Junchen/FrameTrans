# 设置最低cmake版本3.27
cmake_minimum_required(VERSION 3.27)
# 设置项目名称
project(Frame_Rotate)
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
# 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. SRC_LIST)

add_subdirectory(dependencies/minifb)

# 添加可执行文件
add_executable(Frame_Rotate ${SRC_LIST})

# Link MiniFB to your project:
target_link_libraries(Frame_Rotate minifb)