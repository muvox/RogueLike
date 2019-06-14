//
// Created by Master on 13/06/2019.
//

#include "Actor.h"
#include "libtcod.hpp"

Actor::Actor(int x, int y, int ch, const TCODColor &col): x(x),y(y),ch(ch),col(col) {

}

void Actor::render() const {
    static const TCODColor black;
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}


