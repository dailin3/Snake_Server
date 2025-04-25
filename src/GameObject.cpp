//
// Created by dailin on 25-4-18.
//

#include "GameObject.h"
#include "GameItems.h"

int GameObject::maxId = 0;

Point::Point(const int _x, const int _y) : x(_x), y(_y) {}

Point Point::up() const { return Point(x, y+1); }
Point Point::down() const { return Point(x, y-1); }
Point Point::left() const { return Point(x-1, y); }
Point Point::right() const { return Point(x+1, y); }

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

GameObject::GameObject(GameItems* _gameItems ,ObjectType _type, const std::vector<Point> _points)
    : gameItems(_gameItems), type(_type), points(_points) {
    id = ++maxId;
}
