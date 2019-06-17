//
// Created by Master on 13/06/2019.
//

#include "main.h"

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :x(x),y(y),ch(ch),col(col),name(name),attacker(NULL),destructible(NULL),ai(NULL) {}


/* String version instead of char for name
 * Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :x(x),y(y),ch(ch),col(col) { strcpy(this->name, name);}
 */

void Actor::render() const {
    static const TCODColor black;
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() {
    if(ai) ai->update(this);
}




