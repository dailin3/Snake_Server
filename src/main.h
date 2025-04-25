//
// Created by dailin on 25-4-23.
//

#ifndef MAIN_H
#define MAIN_H

#include "Proxy.h"
#include "GameObject.h"
#include "GameItems.h"

std::queue<ReceivedInfo> Proxy::receivedQueue;
std::queue<SendInfo> Proxy::sendQueue;


#endif //MAIN_H
