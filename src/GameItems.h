//
// Created by dailin on 25-4-1.
//

#ifndef GAMEITEMS_H
#define GAMEITEMS_H

#include <iostream>

#include "Player.h"


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

    std::vector<std::shared_ptr<Snake>> getSnakes() const;

    std::vector<std::shared_ptr<Barrier>> getBarriers() const;

    std::vector<std::shared_ptr<Food>> getFoods() const;

    std::shared_ptr<Snake> getSnakeByPlayerId(int id) const;

    void addFood(Point point, int foodScore = 100);

    // add food to a random position
    void addFood(int foodScore = 100);

    // this function should be abandoned
    void addSnake(Point point);

    // add Snake to a random position
    void addSnake(Player* player);

    void addBarrier(Point point);

    void addBarrier(std::vector<Point> points);

    void removeSnakeById(int id);

    void drawItems();

    void update();

    void clear();

    [[nodiscard]] json getGameItemsJson() const;

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
