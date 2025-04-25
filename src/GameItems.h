//
// Created by dailin on 25-4-1.
//

#ifndef GAMEITEMS_H
#define GAMEITEMS_H

#include <iostream>


class Map;
class Point;
class Food;
class Barrier;
class Snake;
class GameObject;

class GameItems {
public:
    int frame = 0;
    GameItems(Map* map);
    ~GameItems();

    const std::vector<std::shared_ptr<Snake>>& getSnakes() const;

    std::vector<std::shared_ptr<Barrier>>& getBarriers();

    std::vector<std::shared_ptr<Food>>& getFoods();

    void addFood(Point point, int foodScore = 100);

    void addSnake(Point point);

    void addBarrier(Point point);

    void removeSnakeById(int id);

    void drawItems();

private:
    Map* map;
    std::vector<std::shared_ptr<Snake>> snakes;
    std::vector<std::shared_ptr<Food>> foods;
    std::vector<std::shared_ptr<Barrier>> barriers;
};

#include "Food.h"
#include "Snake.h"
#include "Barrier.h"

#endif // GAMEITEMS_H
