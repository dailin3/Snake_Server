cmake_minimum_required(VERSION 3.30)
project(SnakeServer)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_INCLUDE thirdparty/boost/1.87.0_1/include)
set(JSON_INCLUDE thirdparty/json-develop/single_include)

include_directories(${BOOST_INCLUDE} ${JSON_INCLUDE} include/)

add_executable(SnakeServer src/main.cpp
        src/Room.cpp
        src/Room.h
        src/GameItems.cpp
        src/GameItems.h
        src/RoomKeeper.cpp
        src/RoomKeeper.h
        src/Proxy.cpp
        src/Proxy.h
        src/Player.cpp
        src/Player.h
        include/Info.h
        src/GameThread.cpp
        src/GameThread.h)
