cmake_minimum_required(VERSION 3.0.0)
project(deamon VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#添加讲需要编译的文件的目录添加到变量
SET(ALL_DIR ${ALL_DIR} ..)
SET(ALL_DIR ${ALL_DIR} ../../CfgParser)
SET(ALL_DIR ${ALL_DIR} ../../CfgParser/yyjson)
SET(ALL_DIR ${ALL_DIR} ../../Common)
# SET(ALL_DIR ${ALL_DIR} Test2/Test3)

#添加讲需要编译的文件添加到变量
foreach(SUB_DIR IN LISTS ALL_DIR)
    FILE(GLOB ALL_FILES ${ALL_FILES} "${SUB_DIR}/*.cpp" "${SUB_DIR}/*.c")  #递归查找该目录下的全部*.cpp、*.c，非递归使用GLOB、递归使用GLOB_RECURSE
    # message(NOTICE "ALL_FILES ...................................${ALL_FILES}")
    get_filename_component(TESTFILE "${SUB_DIR}/test/main.cpp" ABSOLUTE)
    # message(NOTICE "TESTFILE ...................................${TESTFILE}")
    # FILE(REAL_PATH "${SUB_DIR}/main.cpp" TESTFILE)  #获取测试main.cpp文件的绝对路径到TESTFILE, 为了不将测试编入工程
    list(REMOVE_ITEM ALL_FILES ${TESTFILE})         #从ALL_FILES删除测试文件TESTFILE
    include_directories(${SUB_DIR})   #添加全部头文件目录
endforeach()

add_executable(deamon main.cpp ${ALL_FILES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

message(NOTICE "..................................${CMAKE_BUILD_TYPE}")

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









