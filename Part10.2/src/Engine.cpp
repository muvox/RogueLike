#include <math.h>
#include "main.h"

Engine::Engine(int screenWidth, int screenHeight) :
    gameStatus(STARTUP),player(NULL),map(NULL),fovRadius(10),
    screenWidth(screenWidth),screenHeight(screenHeight) {
    TCODConsole::initRoot(screenWidth,screenHeight,"libtcod C++ tutorial",false);
 /*   */
    gui = new Gui();

}

void Engine::init(){
    if(TCODSystem::fileExists("game.sav")){
        TCODSystem::deleteFile("game.sav");
    }
    player = new Actor(40,25,'@',"player",TCODColor::white);
    player->destructible=new PlayerDestructible(30,2,"your cadaver");
    player->attacker=new Attacker(5);
    player->ai = new PlayerAi();
    player->container = new Container(26);
    actors.push(player);
    map = new Map(80,43);
    map->init(true);
    gui->message(TCODColor::red,"Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
    gameStatus=STARTUP;
}

void Engine::save() {
    if( player->destructible->isDead()){
        TCODSystem::deleteFile("game.sav");
    } else {
        TCODZip zip;
        //save the map 1st
        zip.putInt(map->width);
        zip.putInt(map->height);
        map->save(zip);
        //save the player
        player->save(zip);
        //and then the other actors
        zip.putInt(actors.size()-1);
        for(auto *actor : engine.actors){
            if(actor != player) {
                (actor)->save(zip);
            }
        }

        //save the message log
        gui->save(zip);
        zip.saveToFile("game.sav");
    }
}

void Engine::load() {
    engine.gui->menu.clear();
    engine.gui->menu.addItem(Menu::NEW_GAME, "New game");
    if(TCODSystem::fileExists("game.sav")){
        engine.gui->menu.addItem(Menu::CONTINUE,"Continue");
    }
    engine.gui->menu.addItem(Menu::EXIT,"Exit");

    Menu::MenuItemCode menuItem=engine.gui->menu.pick();
    if( menuItem == Menu::EXIT || menuItem == Menu::NONE){
        //exit or window closed
        exit(0);
    } else if( menuItem == Menu::NEW_GAME) {
        //new game
        engine.terminate();
        engine.init();
    } else {
        engine.terminate();
        //load the game
        TCODZip zip;
        zip.loadFromFile("game.sav");
        //load the map
        int width=zip.getInt();
        int height=zip.getInt();
        map = new Map(width, height);
        map->load(zip);

        //load the player
        player=new Actor(0,0,0,NULL, TCODColor::white);
        player->load(zip);
        actors.push(player);

        //then the other actors
        int nbActors=zip.getInt();
        while(nbActors > 0){
            Actor *actor = new Actor(0,0,0,NULL,TCODColor::white);
            actor->load(zip);
            actors.push(actor);
            nbActors--;
        }

        //load the msg log(gui)
        gui->load(zip);
        //to force FOV recomputation
        gameStatus=STARTUP;
    }
}

void Engine::terminate() {
    actors.clearAndDelete();
    if ( map ) delete map;
    gui -> clear();
}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
    delete gui;
}

void Engine::update() {
    if ( gameStatus == STARTUP ) map->computeFov();
    gameStatus=IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);
    if( lastKey.vk == TCODK_ESCAPE ){
        save();
        load();


    }
    player->update();
    if ( gameStatus == NEW_TURN ) {
        for (auto *actor : engine.actors) {
            if ( actor != player ) {
                actor->update();
            }
        }
    }
}

void Engine::render() {
    TCODConsole::root->clear();
    // draw the map
    map->render();
    // draw the actors
    for (auto *actor : engine.actors) {

        if ( actor != player && map->isInFov(actor->x,actor->y) ) {
            actor->render();
        }
    }
    player->render();
    // show the player's stats
    gui->render();
}

void Engine::sendToBack(Actor *actor) {
    actors.remove(actor);
    actors.insertBefore(actor,0);
}

Actor *Engine::getActor(int x, int y) const {
    for (auto *actor : engine.actors) {
        if ( actor->x == x && actor->y ==y && actor->destructible
             && ! actor->destructible->isDead()) {
            return actor;
        }
    }
    return NULL;
}

Actor *Engine::getClosestMonster(int x, int y, float range) const {
    Actor *closest=NULL;
    float bestDistance=1E6f;
    for (Actor **iterator=actors.begin();
         iterator != actors.end(); iterator++) {
        Actor *actor=*iterator;
        if ( actor != player && actor->destructible
             && !actor->destructible->isDead() ) {
            float distance=actor->getDistance(x,y);
            if ( distance < bestDistance && ( distance <= range || range == 0.0f ) ) {
                bestDistance=distance;
                closest=actor;
            }
        }
    }
    return closest;
}

bool Engine::pickATile(int *x, int *y, float maxRange) {
    while ( !TCODConsole::isWindowClosed() ) {
        render();
        // highlight the possible range
        for (int cx=0; cx < map->width; cx++) {
            for (int cy=0; cy < map->height; cy++) {
                if ( map->isInFov(cx,cy) && ( maxRange == 0 || player->getDistance(cx,cy) <= maxRange) ) {
                    TCODColor col=TCODConsole::root->getCharBackground(cx,cy);
                    col = col * 1.2f;
                    TCODConsole::root->setCharBackground(cx,cy,col);
                }
            }
        }
        TCODSystem::checkForEvent(TCOD_EVENT_MOUSE | TCOD_EVENT_KEY_PRESS ,&lastKey,&mouse);
        if ( map->isInFov(mouse.cx,mouse.cy) && ( maxRange == 0 || player->getDistance(mouse.cx,mouse.cy) <= maxRange )) {
            TCODConsole::root->setCharBackground(mouse.cx,mouse.cy,TCODColor::white);
            if ( mouse.lbutton_pressed ) {
                *x=mouse.cx;
                *y=mouse.cy;
                return true;
            }
        }
        if (mouse.rbutton_pressed || lastKey.vk == TCODK_ESCAPE) {
            return false;
        }
        TCODConsole::flush();
    }
    return false;
}
