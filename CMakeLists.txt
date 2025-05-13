cmake_minimum_required(VERSION 3.30)
project(SnakeServer)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_INCLUDE thirdparty/boost/1.87.0_1/include)
set(JSON_INCLUDE thirdparty/json-develop/single_include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${BOOST_INCLUDE} ${JSON_INCLUDE} include/)
aux_source_directory(src/ DIR_SRCS)

add_executable(SnakeServer
        ${DIR_SRCS}
)
