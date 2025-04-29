//
// Created by dailin on 25-4-23.
//

#ifndef MAP_H
#define MAP_H

#include "GameObject.h"

class Cell {
public:
    Cell(int x, int y);
    Cell(Point _p);

    void addObject(std::shared_ptr<GameObject> _gameObject);
    void clearObjects();
    std::vector<std::shared_ptr<GameObject>> getObjects();
    bool isEmpty() const;
private:
    Point position;
    std::vector<std::shared_ptr<GameObject>> objects;
};

class Map {
public:
    Map(int _width, int _height);
    std::vector<Point> getRandomArea(unsigned width, unsigned height, unsigned maxAttempts = 10);
    std::vector<Point> getEdges();

    std::pair<int,int> getSize();
    void clearMap();
    void drawObj(const std::shared_ptr<GameObject>& obj);
    std::string toString();

    int getHeight() const{return height;};
    int getWidth() const{return width;};
private:
    int width, height;
    std::vector<std::vector<Cell*>> map;
};



#endif //MAP_H
