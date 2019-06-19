//
// Created by Master on 15/06/2019.
//

#include "main.h"

bool BspListener::visitNode(TCODBsp *node, void *userData) {
    if (node->isLeaf()) {
        int x, y, w, h;
        //dig a room
        TCODRandom *rng = TCODRandom::getInstance();
        w = rng->getInt(Map::ROOM_MIN_SIZE, node->w - 2);
        h = rng->getInt(Map::ROOM_MIN_SIZE, node->h - 2);
        x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
        y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
        map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);

        if (roomNum != 0){
            //dig corridor from last room
            map.dig(lastx,lasty,x+w/2,lasty);
            map.dig(x+w/2,lasty,x+w/2,y+h/2);
        }

        lastx=x+w/2;
        lasty=y+h/2;
        roomNum++;
    }
    return true;


}