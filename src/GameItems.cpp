//
// Created by dailin on 25-4-1.
//

#include "GameItems.h"
#include "Snake.h"
#include "Barrier.h"
#include "Map.h"

GameItems::GameItems(Map* map): map(map) {

}

GameItems::~GameItems() {

}

const std::vector<std::shared_ptr<Snake>>& GameItems::getSnakes() const {
    return snakes;
}

std::vector<std::shared_ptr<Barrier>>& GameItems::getBarriers() {
    return barriers;
}

std::vector<std::shared_ptr<Food>>& GameItems::getFoods() {
    return foods;
}

void GameItems::addFood(Point point, int foodScore) {
    foods.push_back(std::make_shared<Food>(this, point, foodScore));
}

void GameItems::addSnake(Point point) {
    snakes.push_back(std::make_shared<Snake>(this, point));
}

void GameItems::addBarrier(Point point) {
    barriers.push_back(std::make_shared<Barrier>(this,point));
}

void GameItems::removeSnakeById(int id) {
    auto it = std::find_if(snakes.begin(), snakes.end(), [id](const std::shared_ptr<Snake>& snakePtr) {
        return snakePtr->id == id;
    });
    if (it != snakes.end()) {
        snakes.erase(it);
    }
}

void GameItems::drawItems() {
    //draw snakes;
    for (const auto& s: snakes) {
        map->drawObj(s);
    }
    //draw foods
    for (const auto& f: foods) {
        map->drawObj(f);
    }
    //draw barriers
    for (const auto& b: barriers) {
        map->drawObj(b);
    }
}