#设置cmake的最小支持版本
cmake_minimum_required(VERSION 3.10.2)
#设置项目名称
project(hello_headers)
# 设置资源文件目录
set(SOURCES
        src/Hello.cpp
        src/main.cpp
        )

# 添加可执行文件
add_executable(hello_headers ${SOURCES})
# 设置编译需要被包含的文件目录
target_include_directories(hello_headers
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )