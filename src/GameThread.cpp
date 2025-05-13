//
// Created by dailin on 25-4-8.
//

#include "GameThread.h"


// core logic
void GameThread::handleEachOperation(ReceivedInfo& received_info) {
    GameOperationPayload payload = received_info.getGamePayload();
    int playerId = received_info.getPlayerId();
    auto player = room->getPlayerById(playerId);
    auto ws = received_info.getWSId();

    if (payload.type == GameOperationType::born) {
        room->getGameItems().addSnake(player);
        SendInfo info{ws};
        Proxy::sendQueue.push(info);
        std::cout << room->getGameItems().getGameItemsJson().dump();
    }else if (payload.type == GameOperationType::changeDirection) {
        auto snake = room->getGameItems().getSnakeByPlayerId(playerId);
        if (snake != nullptr) {
            snake->changeDirection(payload.newDirection);
            SendInfo info{ws};
            Proxy::sendQueue.push(info);
        }
    }
    else if (payload.type == GameOperationType::getGameInfo) {
        auto gameJson = room->getGameJson();
        SendInfo info{ws,Responsecode::success,"",gameJson};
        Proxy::sendQueue.push(info);
    }
    else {
        std::cout<< "RoomKeeper error unknown type : " <<static_cast<int>(payload.type) << std::endl;
        SendInfo err_info{ws,Responsecode::failed,"Unknown type"};
        Proxy::sendQueue.push(err_info);
    }
}

void GameThread::snakeMove() {
    for (const auto& snake : room->getGameItems().getSnakes()) {
        snake->move();
    }
}

void GameThread::handleCollision() {
    for (const auto& snake : room->getGameItems().getSnakes()) {
        // TODO: not good strategy.
        snake->judge();
    }
}

void GameThread::bornAndDead() {
    // snake dead or grow
    for (const auto& snake : room->getGameItems().getSnakes()) {
        snake->react();
    }
    // per 5s generate food;
    if (room->frame % 500 == 0) {
        room->getGameItems().addFood();
    }
}


void GameThread::sendResult() {
    // make ws list
    std::vector<int> ws_list;
    for (const auto& player : room->getPlayers()) ws_list.push_back(player->getWSId());

    //make payload
    auto gameJson = this->room->getGameJson();
    // send info
    SendInfo send_info{ws_list,Responsecode::update,"",gameJson};
    Proxy::sendQueue.push(send_info);

    std::cout << room->getMap().toString()<<std::endl;
}

void GameThread::gameEnd() {
    // make ws list
    std::vector<int> ws_list;
    for (const auto& player : room->getPlayers()) {
        ws_list.push_back(player->getWSId());
    };

    SendInfo send_info{ws_list, Responsecode::gameOver};
    Proxy::sendQueue.push(send_info);
    std::cout << "Game End" << std::endl;

    room->clearRoom();
}

void GameThread::start() {
    if (thread.joinable()) {
        thread.join();
    }
    thread = std::thread(&GameThread::gameLoop, this);
}

void GameThread::join() {
    thread.join();
}

GameThread::GameThread(Room *room): room(room) {
}

Room * GameThread::getRoom() const{
    return room;
}

void GameThread::initGame() {
    // init map and gameItems
    room->initMap();
    room->initWall();
    room->initSnake();
}

void GameThread::gameEachLoop() {
    // handle the info from player.
    for (auto& operation : room->getOperationsList()) handleEachOperation(operation);
    // move snake.
    snakeMove();
    // make a judgement for the collision.
    handleCollision();
    // born and dead
    bornAndDead();
    // update game
    room->getGameItems().update();
    // send result to players.
    sendResult();
    // game frame ++
    room->frame += 1;
}

void GameThread::gameLoop() {
    room->setRoomState(RoomState::InitGaming);
    initGame();
    room->setRoomState(RoomState::Playing);
    while (room->getState()==RoomState::Playing) {
        gameEachLoop();
        // game end.
        if (room->frame == room->getGameAllFrames()) {
            room->setRoomState(RoomState::Readying);
            gameEnd();
        }
        // wait for 0.1s
        std::this_thread::sleep_for(std::chrono::milliseconds(room->getFreshMiliSeconds()));
    }
}


