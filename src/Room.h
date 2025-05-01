// Room.h
#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
#include <queue>
#include <vector>

#include "GameItems.h"
#include "GameThread.h"
#include "Info.h"
#include "Map.h"

class GameThread;
class Player;

enum class RoomState {
    Readying,
    InitGaming,
    Playing,
};

class Room {
public:
    Room();

    [[nodiscard]] int getId() const;
    [[nodiscard]] RoomState getState() const;
    [[nodiscard]] GameThread* getGameThread() const;
    [[nodiscard]] GameItems getGameItems() const;
    [[nodiscard]] Map getMap() const;
    [[nodiscard]] std::vector<Player*> getPlayers() const;
    [[nodiscard]] Player* getPlayerById(int id);
    [[nodiscard]] json getGameInfo() const;

    void setGameThread(GameThread* gameThread);
    void setRoomState(RoomState roomState);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    void pushOperations(const ReceivedInfo& operation);
    std::vector<ReceivedInfo> getOperationsList();

    void initMap(int width = 25, int height = 25);
    void initGameItems();
    void initWall();
    void initSnake();

    int startGame(Player* player);
    int stopGame(Player* player);
    void overGame();

    int frame = 0;

    [[nodiscard]] json getRoomJson() const;

    [[nodiscard]] json getGameJson() const;

    [[nodiscard]] std::vector<json> getPlayersJson() const;

private:
    static int maxRoomId;
    int roomId;
    RoomState state;
    std::vector<Player*> players = std::vector<Player*>();
    GameThread* gameThread = nullptr;
    std::queue<ReceivedInfo> operationsQueue;
    GameItems gameItems;
    Map map;
};

#endif // ROOM_H