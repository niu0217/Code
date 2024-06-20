cmake_minimum_required(VERSION 2.6)

project(SimpleNet)

# 启用测试
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
 -g
 -Wall
 -Wextra
 -Werror # 把警告当成错误
 -Wconversion # 一些可能改变值的隐式转换，给出警告
 -Wno-unused-parameter # 函数中出现未使用的参数，不给出警告
 -Wold-style-cast # C风格的转换，给出警告
 -Woverloaded-virtual # 如果函数的声明隐藏住了基类的虚函数，就给出警告
 -Wpointer-arith # 对函数指针或者void*类型的指针进行算术操作时给出警告
 -Wshadow # 当一个局部变量覆盖住了另一个局部变量或者全局变量时，给出警告
 -Wwrite-strings # 规定字符串常量的类型为const char[length],把这样的地址复制给non-const char*指针会产生警告
 # -MMD
 -std=c++11
 -rdynamic
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
# 特定于Clang编译器的编译选项
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference") # 禁止在空指针解引用时的警告
  list(APPEND CXX_FLAGS "-Wno-sign-conversion") # 禁止有关有符号和无符号类型之间转换的警告
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef") # 禁止关于未使用的本地类型定义（typedef）的警告
  list(APPEND CXX_FLAGS "-Wthread-safety") # 启用线程安全分析
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED) # 查找并加载Boost库
# 在Boost库中查找名为 boost_unit_test_framework 的库文件
find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(base)


# 
# 编译可执行文件
# 
add_executable(client client.cpp)
target_link_libraries(client io_base)