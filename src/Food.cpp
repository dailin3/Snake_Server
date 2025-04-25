//
// Created by dailin on 25-4-18.
//

#include "Food.h"

Food::Food(GameItems* gameItems, Point point, int foodScore)
    : foodScore(foodScore), GameObject(gameItems, ObjectType::food,{point}) {
}