//
// Created by dailin on 25-4-18.
//

#ifndef BARRIER_H
#define BARRIER_H

#include "GameObject.h"

class Barrier : public GameObject {
public:
    Barrier(GameItems* gameItems, Point point);
    Barrier(GameItems* gameItems, std::vector<Point> points);
};

#endif // BARRIER_H
