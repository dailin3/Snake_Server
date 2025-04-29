//
// Created by dailin on 25-4-23.
//

#include "Map.h"

#include <random>


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

bool Cell::isEmpty() const {
    return objects.empty();
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

std::vector<Point> Map::getRandomArea(unsigned width, unsigned height, unsigned maxAttempts) {
    // Set up random number generation
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed); // Mersenne Twister engine
    // Define distributions for random x and y coordinates within map bounds
    std::uniform_int_distribution<unsigned> distrib_x(0, getWidth() - width - 1);
    std::uniform_int_distribution<unsigned> distrib_y(0, getHeight() -height - 1);

    auto isAvailableBlock = [this, width, height](Point p) {

        // if the block is out map
        if (p.x + width > getWidth() || p.y + height > getHeight()) return false;

        //if the block is empty
        for (int dy = 0; dy < width; dy++) {
            for (int dx = 0; dx < height; dx++) {
                if (!map[p.x + dx][p.y + dy]->isEmpty()) {
                    return false;
                }
            }
        }
        return true;
    };

    auto generatePoints = [width, height](Point p) {
        std::vector<Point> points{};
        for (int dy = 0; dy < height; dy++) {
            for (int dx = 0; dx < width; dx++) {
                points.push_back(Point(p.x + dx, p.y + dy));
            }
        }

        return points;
    };

    for (int i = 0; i < maxAttempts; i++) {
        unsigned startX = distrib_x(rng);
        unsigned startY = distrib_y(rng);
        Point p(static_cast<int>(startX), static_cast<int>(startY));
        if (isAvailableBlock(p) ){
            return generatePoints(p);
        }
    }

    return std::vector<Point>({Point(-1,-1)});
}

std::vector<Point> Map::getEdges() {
    auto edges = std::vector<Point>();
    for (int i=0; i < this->getWidth(); i++) {
        edges.push_back(Point(i, 0));
        edges.push_back(Point(i, this->getHeight() - 1));
    }
    for (int i=1; i < this->getHeight()-1; i++) {
        edges.push_back(Point(0, i));
        edges.push_back(Point(this->getWidth() - 1, i));
    }
    return edges;
}
