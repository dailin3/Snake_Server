//
// Created by dailin on 25-4-18.
//

#include "Snake.h"

Snake::Snake(GameItems* gameItems, Point point)
    : header(point), GameObject(gameItems, ObjectType::snake, {point}) {
    direction = Direction::down;
    snakeLength = 10;
}

Snake::Snake(GameItems *gameItems,Player* player ,std::vector<Point> points, Point header, int snakeLength, Direction direction)
    : header(header), GameObject(gameItems, ObjectType::snake, points), direction(direction), snakeLength(snakeLength), player(player) {
}

void Snake::changeDirection(Direction _direction) {
    if (validChangeDirection(_direction, this->direction)) {
        this->direction = _direction;
    }
}

void Snake::move() {
    points.pop_back();  // delete the last element
    switch (this->direction) {

        // create new header
        case Direction::up:
            header = header.up();
            break;
        case Direction::down:
            header = header.down();
            break;
        case Direction::left:
            header = header.left();
            break;
        case Direction::right:
            header = header.right();
            break;
    }

    points.insert(points.begin(), header); // add header to first
}

void Snake::grow(int length) {
    this->snakeLength += length;
    auto tailDirection = Direction::up;
    auto addPoint = *(points.rbegin());

    // judge the tail direction
    if (points.size() > 1) {
        auto lastPoint = *(points.rbegin());
        auto secondLastPoint = *(points.rbegin() + 1);
        int dx = lastPoint.x - secondLastPoint.x;
        int dy = lastPoint.y - secondLastPoint.y;
        if (dx==1) tailDirection = Direction::right;
        else if (dx==-1) tailDirection = Direction::left;
        else if (dy==1) tailDirection = Direction::down;
        else if (dy==-1) tailDirection = Direction::up;
    }
    switch (tailDirection) {
        // create new header
        case Direction::up:
            addPoint = addPoint.up();
            break;
        case Direction::down:
            addPoint = addPoint.down();
            break;
        case Direction::left:
            addPoint = addPoint.left();
            break;
        case Direction::right:
            addPoint = addPoint.right();
            break;
    }

    points.insert(points.end(), addPoint); // add header to first
    setStatus(SnakeStatus::alive);
}

void Snake::dead() {
    for (auto point : points) {
        gameItems->addFood(point, 100);
    }
    gameItems->removeSnakeById(id);
}

void Snake::react() {
    if (snakeStatus == SnakeStatus::alive) return;
    if (snakeStatus == SnakeStatus::dead) dead();
    if (snakeStatus == SnakeStatus::grow) grow();
}

Snake::SnakeStatus Snake::setStatus(const SnakeStatus status) {
    this->snakeStatus = status;
    return snakeStatus;
}

Player * Snake::getPlayer() const {
    return this->player;
}

Point Snake::getHeader() const {
    return this->header;
}

bool Snake::validChangeDirection(Direction target, Direction now) {
    return true;
}
