#include "main.h"
#include "Map.h"
#include "Snake.h"
#include "GameItems.h"
#include "Player.h"

#include <iostream>

#include "GameThread.h"
#include "Room.h"

int main() {
    // test Proxy
    asio::io_context io;
    Proxy proxy{io,1145};
    std::thread proxyThread(&Proxy::begin, &proxy);

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

    // test Room and GameThread.
    Room room;
    Player p{"dora",1,PlayerState::InRoom,nullptr};
    room.addPlayer(&p);

    // bind both
    auto gt = new GameThread(&room);
    room.setGameThread(gt);

    gt->initGame();
    std::cout << room.getRoomId() << std::endl;
    std::cout << room.getMap().toString();

    nlohmann::json testjson{
        {"type", 1},
        {"roomId", 0},
        {"playerId",1},
        {"payload",{"data","up"}}
    };
    ReceivedInfo reinfo{testjson,nullptr};
    room.pushOperations(reinfo);

    //gt->gameEachLoop();

    proxyThread.join();
    return 0;
}
