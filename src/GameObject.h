//
// Created by dailin on 25-4-18.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
class GameItems;

enum class Direction {
    up=0, down=1, left=2, right=3
};

class Point {
public:
    Point(const int _x, const int _y) : x(_x), y(_y) {}
    Point up() const { return Point(x, y+1); }
    Point down() const { return Point(x, y-1); }
    Point left() const { return Point(x-1, y); }
    Point right() const { return Point(x+1, y); }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

private:
    int x,y=0;
};

class GameObject {
public:
    GameObject(GameItems* _gameItems ,std::string _type, const std::vector<Point> &_points): allItems(_gameItems),type(std::move(_type)), points(_points) {}
    std::vector<Point> getPoints() {
        return points;
    };
private:
    std::string type;
    std::vector<Point> points;
    GameItems* allItems;
};

class Cell {
private:
    Point position;
    std::vector<GameObject*> objects;
};

class Map {
private:
    std::array<std::array<Cell, 128>, 128> map;
};

#include "GameItems.h"

#endif //GAMEOBJECT_H
