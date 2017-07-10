cmake_minimum_required(VERSION 2.8)
project(mongoose_example)

set(CMAKE_CXX_STANDARD 11)

#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

#配置编译选项
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb -Wall")
#配置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -pthread -DMG_ENABLE_THREADS")

set(SOURCE_FILES main.cpp)


#配置头文件目录
include_directories("./easylogger/include")
include_directories("./include")

#添加源文件目录

aux_source_directory(./easylogger/ SOURCE_FILES)
aux_source_directory(./src SOURCE_FILES)

include_directories(

)

link_directories("/usr/local/lib/")


set(LIBRARIES

)


add_executable(mongoose_example ${SOURCE_FILES})
target_link_libraries(mongoose_example ${LIBRARIES})