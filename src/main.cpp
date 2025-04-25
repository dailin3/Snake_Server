#include "main.h"
#include "Map.h"
#include "Snake.h"
#include "GameItems.h"

#include <iostream>

int main() {
    // asio::io_context io;
    // Proxy p{io,1145};
    auto map = Map(10,10);
    auto gameItems = GameItems(&map);
    gameItems.addSnake({2, 2});
    gameItems.addBarrier({3, 7});
    gameItems.addFood({0,0});

    gameItems.drawItems();
    std::cout << map.toString();

    std::cout << "---" <<std::endl;

    auto s = gameItems.getSnakes()[0];
    s -> move();
    s -> grow();
    s -> dead();
    map.clearMap();
    gameItems.drawItems();
    std::cout << map.toString();
    return 0;
}