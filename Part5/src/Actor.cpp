//
// Created by Master on 13/06/2019.
//

#include "Actor.h"
#include "libtcod.h"
#include "Map.h"
#include "Engine.h"

Actor::Actor(int x, int y, int ch, const TCODColor &col): x(x),y(y),ch(ch),col(col) {

}

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :x(x),y(y),ch(ch),col(col) {}


/* String version instead of char for name
 * Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :x(x),y(y),ch(ch),col(col) { strcpy(this->name, name);}
 */

void Actor::render() const {
    static const TCODColor black;
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}

void Actor::update() {
//    printf("The %s growls\n", name);
// could not get printf to work, it just crashed the program on every step i took.
}

bool Actor::moveOrAttack(int x, int y) {
    if ( engine.map->isWall(x,y)) return false;

    for(auto *a : engine.actors){
        if( a->x == x && a-> y == y){
//            printf("The %s laughs at your puny efforts to attack him! \n", a->name);
// could not get printf to work, it just crashed the program on every step i took.
            string message = "The %s laughs at your puny efforts to attack him!";
            cout << message << endl;
            return false;
        }
    }

    this->x=x;
    this->y=y;
    return true;
}

/*
 *
 *             string error = "Error Initializing";
            SDL_Log(error.c_str());
            SDL_Log(SDL_GetError());
 */
