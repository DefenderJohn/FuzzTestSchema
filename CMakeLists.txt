# 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和（可选的）版本号
project(FuzzTestSchema)

set(CMAKE_CXX_STANDARD 17)

# 如果有特定编译器标志要设置，可以在这里设置
add_compile_options(-O3 -g3 -ggdb)

# 添加 include 目录
include_directories(${PROJECT_SOURCE_DIR}/src/include)

add_executable(FuzzTestSchemaExample ${PROJECT_SOURCE_DIR}/example/main.cpp)

