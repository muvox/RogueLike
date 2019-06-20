//
// Created by Master on 17/06/2019.
//

#include "main.h"

Container::Container(int size) : size(size){


}

Container::~Container() {
    inventory.clearAndDelete();
}

void Container::load(TCODZip &zip) {
    size=zip.getInt();
    int nbActors=zip.getInt();
    while ( nbActors > 0){
        Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
        actor->load(zip);
        inventory.push(actor);
        nbActors--;
    }
}

void Container::save(TCODZip &zip) {
    zip.putInt(size);
    zip.putInt(inventory.size());
    for(auto actor : inventory){
        actor->save(zip);
    }
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