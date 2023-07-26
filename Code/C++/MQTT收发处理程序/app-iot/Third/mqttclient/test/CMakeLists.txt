cmake_minimum_required(VERSION 3.0.0)
project(mqtt VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#添加讲需要编译的文件的目录添加到变量
SET(ALL_DIR ${ALL_DIR} ..)
SET(ALL_DIR ${ALL_DIR} ../include)
SET(ALL_DIR ${ALL_DIR} ../include/mqtt)
SET(ALL_DIR ${ALL_DIR} ../include/mqttclient)
SET(ALL_DIR ${ALL_DIR} ../include/common)
SET(ALL_DIR ${ALL_DIR} ../include/common/log)
SET(ALL_DIR ${ALL_DIR} ../include/platform/linux)
SET(ALL_DIR ${ALL_DIR} ../include/test)
SET(ALL_DIR ${ALL_DIR} ../include/network)

#添加讲需要编译的文件添加到变量
foreach(SUB_DIR IN LISTS ALL_DIR)
    FILE(GLOB ALL_FILES ${ALL_FILES} "${SUB_DIR}/*.cpp" "${SUB_DIR}/*.c")  #递归查找该目录下的全部*.cpp、*.c，非递归使用GLOB、递归使用GLOB_RECURSE
    get_filename_component(TESTFILE "${SUB_DIR}/test/main.cpp" ABSOLUTE)
    list(REMOVE_ITEM ALL_FILES ${TESTFILE})         #从ALL_FILES删除测试文件TESTFILE
    include_directories(${SUB_DIR})       #添加全部头文件目录 
endforeach()

# 添加查找动态库
SET(LIB_DIR ${LIB_DIR} ../lib/pc)
find_library(libvar mqttclient ${LIB_DIR}) 

add_executable(${PROJECT_NAME} main.cpp ${ALL_FILES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(${PROJECT_NAME} ${libvar})   #添加动态静态库
message(NOTICE "..................................${libvar}")

# message打印函数级别

# FATAL_ERROR         cmake出错, 停止编译和生成(信息红色)
# SEND_ERROR          cmake出错, 继续编译, 但是停止生成(信息红色)
# WARNING             cmake警告, 继续编译(信息红色)
# AUTHOR_WARNING      开发者警告, 继续编译(信息红色)
# DEPRECATION         如果使用set方法设置CMAKE_ERROR_DEPRECATED为true(不区分大小写), 编译出错, 否则继续编译
# NOTICE              不设置mode, 默认是NOTICE模式, 不影响编译和生成, 用于打印消息(信息白色)
# STATUS              编译时状态信息, 左边以--开头(信息白色)
# DEBUG               针对开发人员的调试信息(信息白色)
# TRACE               日志级别的临时信息(信息白色)


