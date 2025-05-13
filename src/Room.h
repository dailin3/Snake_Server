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
    [[nodiscard]] GameItems& getGameItems();
    [[nodiscard]] Map getMap() const;
    [[nodiscard]] std::vector<Player*> getPlayers() const;
    [[nodiscard]] Player* getPlayerById(int id);
    [[nodiscard]] int getGameAllFrames() const;
    [[nodiscard]] int getFreshMiliSeconds() const;

    void setGameThread(GameThread* gameThread);
    void setRoomState(RoomState roomState);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    void pushOperations(const ReceivedInfo& operation);
    std::vector<ReceivedInfo> getOperationsList();

    void initMap(int width = 50, int height = 50);
    void initGameItems();
    void initWall();
    void initSnake();

    int startGame(Player* player);
    int stopGame(Player* player);
    void overGame();

    void clearRoom();

    int frame = 0;

    [[nodiscard]] json getRoomJson() const;

    [[nodiscard]] json getGameJson() const;

    [[nodiscard]] std::vector<json> getPlayersJson() const;

    int getOwnerId() const;

private:
    static int maxRoomId;
    int roomId;
    RoomState state;
    std::vector<Player*> players = std::vector<Player*>();
    GameThread* gameThread = nullptr;
    std::queue<ReceivedInfo> operationsQueue;
    GameItems gameItems;
    Map map;
    int gameAllFrames = 1800;
    int freshMiliSeconds = 100;
    int ownerId = 0;
};

#endif // ROOM_H