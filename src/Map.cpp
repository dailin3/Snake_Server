//
// Created by dailin on 25-4-23.
//

#include "Map.h"


Cell::Cell(int x, int y): position(x,y) {
    objects = std::vector<std::shared_ptr<GameObject>>();
}

Cell::Cell(Point _p): position(_p) {
    objects = std::vector<std::shared_ptr<GameObject>>();
}

void Cell::addObject(std::shared_ptr<GameObject> _gameObject) {
    objects.push_back(_gameObject);
}

void Cell::clearObjects() {
    objects.clear();
}

std::vector<std::shared_ptr<GameObject>> Cell::getObjects() {
    return objects;
}

Map::Map(int _width, int _height): width(_width), height(_height) {
    map.resize(_height, std::vector<Cell*>(_width,nullptr));
    // init every cell for map
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            map[x][y] = new Cell(x,y);
        }
    }
}

std::pair<int,int> Map::getSize() {
    return std::make_pair(width, height);
}

void Map::clearMap() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map[x][y]->clearObjects();
        }
    }
}

void Map::drawObj(const std::shared_ptr<GameObject>& obj) {
    for (const auto p : obj->points) {
        map[p.x][p.y]->addObject(obj);
    }
}

// to debug function
std::string Map::toString() {
    std::string str;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto objs = map[x][y]->getObjects();
            if (objs.size() > 0) {
                switch (objs[0]->type) {
                    case ObjectType::snake:
                        str += "@";break;
                    case ObjectType::food:
                        str += "*";break;
                    case ObjectType::barrier:
                        str += "#";break;
                }
            }
            // the cell have no obj
            else {
                str += ".";
            }
        }
        str += "\n";
    }
    return str;
}
