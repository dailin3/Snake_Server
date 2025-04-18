//
// Created by dailin on 25-4-18.
//

#ifndef SNAKE_H
#define SNAKE_H

#include "GameObject.h"
#include "Barrier.h"
#include "Food.h"

class Snake : public GameObject {
    enum class SnakeStatus {
        alive = 0,
        dead = 1,
        grow = 2,
    };

    Snake(GameItems* gameItems ,Point point): header(point), GameObject(gameItems,"Snake", std::vector<Point>{point}) {
        direction = Direction::down;
        snakeLength = 10;
    }

    void changeDirection(Direction _direction) {
        this -> direction = _direction;
    }

    void move() {
        switch (this -> direction) {
            points.pop_back();  // delete the last element

            // create new header
            case Direction::up:
                header  = header.up();
            case Direction::down:
                header  = header.down();
            case Direction::left:
                header  = header.left();
            case Direction::right:
                header  = header.right();

            points.insert(points.begin(), header); // add header to first
        }
    }

    void grow(int length = 10) {
        this -> snakeLength += length;
        switch (this -> direction) {
            // create new header
            case Direction::up:
                header  = header.up();
            case Direction::down:
                header  = header.down();
            case Direction::left:
                header  = header.left();
            case Direction::right:
                header  = header.right();

            points.insert(points.begin(), header); // add header to first
        }
    }

    void dead() {
        for (auto point : points) {
            auto food = Food{gameItems ,point, 100};
            gameItems->addFood(food);
        }
        delete this;
    }

    void judge() {
        // snake to snake
        for (auto& snake : gameItems -> getSnakes()) {
            if (snake.get() != this) {    // if the snake is not this snake self
                for (Point point : snake -> getPoints()) {
                    if (header == point) {  // if collision happens
                        this -> setStatus(SnakeStatus::dead);
                    }
                }
            }
        }

        // snake to wall
        for (auto& barrier : gameItems -> getBarriers()) {
            for (Point point : barrier -> getPoints()) {
                if (header == point) {
                    this -> setStatus(SnakeStatus::dead);
                }
            }
        }

        // snake to food
        for (auto& food : gameItems -> getFoods()) {
            for (Point point : food -> getPoints()) {
                if (header == point) {
                    this -> setStatus(SnakeStatus::grow);
                }
            }
        }
    }

    void react() {
        if (snakeStatus == SnakeStatus::alive) return;
        if (snakeStatus == SnakeStatus::dead) dead();
        if (snakeStatus == SnakeStatus::grow) grow();
    }


    SnakeStatus setStatus(const SnakeStatus status) {
        this -> snakeStatus = status;
    }

private:
    Point header;
    std::vector<Point> points;
    Direction direction;
    int snakeLength;
    GameItems* gameItems;
    SnakeStatus snakeStatus;
};




#endif //SNAKE_H
