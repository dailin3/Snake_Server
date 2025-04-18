//
// Created by dailin on 25-4-1.
//

#ifndef GAMEITEMS_H
#define GAMEITEMS_H

#include <memory>
#include <iostream>

#include "Food.h"

class Food;
class Snake;
class Barrier;
class Map;

class GameItems {
public:
    int frame = 0;
    GameItems();
    ~GameItems();

    const std::vector<std::unique_ptr<Snake>>& getSnakes() const {
         return snakes;
    };

    std::vector<std::unique_ptr<Barrier>>& getBarriers() {
        return barriers;
    };

    std::vector<std::unique_ptr<Food>>& getFoods() {
        return foods;
    }

    void addFood(Food& food) {
        foods.push_back(std::make_unique<Food>(food));
    }

private:
    Map* map;
    std::vector<std::unique_ptr<Snake>> snakes;
    std::vector<std::unique_ptr<Food>> foods;
    std::vector<std::unique_ptr<Barrier>> barriers;
};



#endif //GAMEITEMS_H
