#include "main.h"

Engine engine(80,50);

int main() {
    engine.load();
    while(!TCODConsole::isWindowClosed() ){
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    engine.save();

    return 0;
}