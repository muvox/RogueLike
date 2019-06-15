//
// Created by Master on 15/06/2019.
//

#ifndef MOGUE_BSPLISTENER_H
#define MOGUE_BSPLISTENER_H


#include <libtcod.h>
#include "Map.h"
#include "Actor.h"
#include "Engine.h"

class BspListener : public ITCODBspCallback {
private:
    Map &map; //map to dig
    int roomNum; //room number
    int lastx, lasty; //center of the last room
public:
    BspListener(Map &map) : map(map), roomNum(0) {}

    bool visitNode(TCODBsp *node, void *userData);
};


#endif //MOGUE_BSPLISTENER_H
