//
// Created by dailin on 25-4-18.
//

#ifndef FOOD_H
#define FOOD_H

#include "GameObject.h"

class Food : public GameObject {
    public:
    Food(GameItems* gameItems ,Point point, int foodScore):foodScore(foodScore), GameObject(gameItems,"Food", std::vector<Point>{point}) {
    }
    private:
    int foodScore;
};


#endif //FOOD_H
