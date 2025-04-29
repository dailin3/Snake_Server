#include "main.h"
#include "Map.h"
#include "Snake.h"
#include "GameItems.h"
#include "Player.h"

#include <iostream>

#include "Room.h"

int main() {
    // test Proxy
    // asio::io_context io;
    // Proxy p{io,1145};
    // p.begin();

    // // test GameItems
    // auto map = Map(15,15);
    // auto gameItems = GameItems(&map);
    // gameItems.addSnake({2, 2});
    // gameItems.addBarrier({3, 7});
    //
    // gameItems.update();
    // std::cout << map.toString();
    //
    // std::cout << "---" <<std::endl;
    //
    // auto s = gameItems.getSnakes()[0];
    // s -> move();
    // s -> grow();
    // s -> dead();
    // gameItems.update();
    // std::cout << map.toString();

    // // test generate randomly
    // std::cout << "---" <<std::endl;
    // Player p{"dora",1,PlayerState::InRoom,nullptr};
    // gameItems.addSnake(&p);
    // gameItems.addFood();
    // gameItems.addBarrier(map.getEdges());
    // gameItems.update();
    // std::cout << map.toString();

    // // test Room init
    // Room room;
    // Player p{"dora",1,PlayerState::InRoom,nullptr};
    // room.addPlayer(&p);
    // room.initMap();
    // room.initWall();
    // room.initSnake();
    // std::cout << room.getMap().toString();

    return 0;
}
