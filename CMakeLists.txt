cmake_minimum_required(VERSION 2.8)
project(mongoose_example)

#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_BUILD_TYPE Debug)

#配置编译选项
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb -Wall")


include_directories(
        include
)

set(SRC
        src/mongoose.c
        main.c
        )


add_executable(mongoose_example
        ${SRC}
        )

add_definitions("-DMG_ENABLE_HTTP_STREAMING_MULTIPART")

target_link_libraries(mongoose_example
        m pthread crypt
        )