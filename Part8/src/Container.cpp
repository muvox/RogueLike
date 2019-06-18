//
// Created by Master on 17/06/2019.
//

#include "main.h"

Container::Container(int size) : size(size){


}

Container::~Container() {
    inventory.clearAndDelete();
}

bool Container::add(Actor *actor) {
    if ( size > 0 && inventory.size() >= size){
        //inventory is full
        return false;
    }

    inventory.push(actor);
    return true;
}

void Container::remove(Actor *actor) {
    if(inventory.contains(actor)){
        inventory.remove(actor);
    } else {
        engine.gui->message(TCODColor::red, "Container has no actor %s", actor->name);
    }
}