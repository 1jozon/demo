cmake_minimum_required(VERSION 3.10)
project(sylar_network LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 获取当前目录的绝对路径
get_filename_component(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} REALPATH)

# 收集所有 .cpp 文件（返回绝对路径）
file(GLOB ALL_CPP_FILES "${SRC_DIR}/*.cpp")

# 调试：打印所有文件
message(STATUS "🔍 所有 .cpp 文件: ${ALL_CPP_FILES}")

# 定义要排除的文件（必须用完整路径）
set(EXCLUDE_FILES
    ${SRC_DIR}/server.cpp
    ${SRC_DIR}/client.cpp
)

# 从列表中移除
set(COMMON_SOURCES ${ALL_CPP_FILES})
list(REMOVE_ITEM COMMON_SOURCES ${EXCLUDE_FILES})

# 调试：打印公共源文件
message(STATUS "✅ 公共源文件 (不含 main): ${COMMON_SOURCES}")

# 生成可执行文件
add_executable(server server.cpp ${COMMON_SOURCES})
add_executable(client client.cpp ${COMMON_SOURCES})

# 链接系统库
target_link_libraries(server pthread dl rt)
target_link_libraries(client pthread dl rt)