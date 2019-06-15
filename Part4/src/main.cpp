#include <iostream>
#include "libtcod.h"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"

Engine engine;

int main() {

    while(!TCODConsole::isWindowClosed() ){
        engine.update();
        engine.render();
        TCODConsole::flush();
    }

    return 0;
}