//
// Created by dailin on 25-4-18.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <nlohmann/json.hpp>

class GameItems;


class Point {
public:
    Point(const int _x, const int _y);
    Point up() const;
    Point down() const;
    Point left() const;
    Point right() const;

    bool operator==(const Point& other) const;

    int x,y=0;
};

enum class Direction {
    up=0, down=1, left=2, right=3
};

// void =0;
enum class ObjectType {
    snake = 1,
    food = 2,
    barrier = 3,
};

class GameObject {
public:
    static int maxId;

    GameObject(GameItems* _gameItems ,ObjectType _type, const std::vector<Point> _points);

    [[nodiscard]] nlohmann::json to_json() const;

    std::vector<Point> points;
    int id;
    ObjectType type;
    GameItems* gameItems;
};

#endif //GAMEOBJECT_H
