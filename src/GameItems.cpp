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

std::vector<std::shared_ptr<Snake>> GameItems::getSnakes() const {
    return snakes;
}

std::vector<std::shared_ptr<Barrier>> GameItems::getBarriers() const{
    return barriers;
}

std::vector<std::shared_ptr<Food>> GameItems::getFoods() const{
    return foods;
}

std::shared_ptr<Snake> GameItems::getSnakeByPlayerId(int id) const {
    for (auto& snake : snakes) {
        if (snake->getPlayer()->getId() == id) {return snake;}
    }
    return nullptr;
}

void GameItems::addFood(Point point, int foodScore) {
    foods.push_back(std::make_shared<Food>(this, point, foodScore));
}

void GameItems::addFood(int foodScore) {
    auto points =  this->map->getRandomArea(1,1);
    foods.push_back(std::make_shared<Food>(this, points ,foodScore));
}

void GameItems::addSnake(Point point) {
    snakes.push_back(std::make_shared<Snake>(this, point));
}

void GameItems::addSnake(Player* player) {
    auto points =  this->map->getRandomArea(1,2);
    auto direction = Direction::up;
    auto header = points[0];
    auto snakeLength = 20;
    snakes.push_back(std::make_shared<Snake>(this, player ,points, header, snakeLength,direction));
}

void GameItems::addBarrier(Point point) {
    barriers.push_back(std::make_shared<Barrier>(this,point));
}

void GameItems::addBarrier(std::vector<Point> points) {
    barriers.push_back(std::make_shared<Barrier>(this, points));
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

void GameItems::update() {
    map->clearMap();
    drawItems();
}

void GameItems::clear() {
    snakes.clear();
    foods.clear();
    barriers.clear();
}

json GameItems::getGameItemsJson() const {
    auto snake_array = json::array();
    auto food_array = json::array();
    auto barrier_array = json::array();
    for (auto snake : snakes){
        auto snake_json = snake->to_json();
        snake_json["playerId"] = snake->getPlayer()->getId();
        snake_json["playerName"] = snake->getPlayer()->getName();
        snake_array.push_back(snake_json);
    }
    for (auto food : foods) {
        auto food_json = food->to_json();
        food_array.push_back(food_json);
    }
    for (auto barrier : barriers) {
        auto barrier_json = barrier->to_json();
        barrier_array.push_back(barrier_json);
    }

    return json{
        {"snakes",snake_array},
        {"foods",food_array},
        {"barriers",barrier_array},
    };
}
