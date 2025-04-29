#include "main.h"
#include "Map.h"
#include "Snake.h"
#include "GameItems.h"
#include "Player.h"

#include <iostream>

int main() {
    // test Proxy
    // asio::io_context io;
    // Proxy p{io,1145};
    // p.begin();
    // // test GameItems
    // auto map = Map(10,10);
    // auto gameItems = GameItems(&map);
    // gameItems.addSnake({2, 2});
    // gameItems.addBarrier({3, 7});
    // gameItems.addFood({0,0});
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
    //
    // // test generate randomly
    // std::cout << "---" <<std::endl;
    // Player p{"dora",1,PlayerState::InRoom,nullptr};
    // gameItems.addSnake(&p);
    // gameItems.addFood();
    // gameItems.update();
    // std::cout << map.toString();

    return 0;
}