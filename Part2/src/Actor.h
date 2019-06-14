//
// Created by Master on 13/06/2019.
//

#ifndef MOGUE_ACTOR_H
#define MOGUE_ACTOR_H
#include "libtcod.hpp"

class Actor {
public:
    int x, y;//position
    int ch; //ascii code
    TCODColor col;
public:
    Actor(int x, int y, int ch, const TCODColor &col);
    void render() const;
};


#endif //MOGUE_ACTOR_H
