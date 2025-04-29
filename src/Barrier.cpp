//
// Created by dailin on 25-4-18.
//

#include "Barrier.h"

Barrier::Barrier(GameItems* gameItems, Point point)
    : GameObject(gameItems, ObjectType::barrier, {point}) {
}

Barrier::Barrier(GameItems *gameItems, std::vector<Point> points)
    : GameObject(gameItems, ObjectType::barrier, points) {
}