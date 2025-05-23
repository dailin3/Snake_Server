//
// Created by dailin on 25-4-18.
//

#ifndef SNAKE_H
#define SNAKE_H

#include "GameItems.h"
#include "GameObject.h"
#include "Barrier.h"
#include "Food.h"
#include "Player.h"

class Snake : public GameObject {
public:
    enum class SnakeStatus {
        alive = 0,
        dead = 1,
        grow = 2,
    };
    Snake(GameItems* gameItems, Point point);   // just for test
    Snake(GameItems* gameItems, Player* player, std::vector<Point> points, Point header, int snakeLength, Direction direction = Direction::up);
    void changeDirection(Direction _direction);
    void move();
    void grow(int length = 10);
    void dead();
    void judge();
    void react();
    SnakeStatus setStatus(const SnakeStatus status);
    Player* getPlayer() const;
    Point getHeader() const;

private:
    Player* player = nullptr;
    Point header;
    Direction direction;
    int snakeLength;
    SnakeStatus snakeStatus;
    static bool validChangeDirection(Direction target,Direction now);
};

#endif // SNAKE_H