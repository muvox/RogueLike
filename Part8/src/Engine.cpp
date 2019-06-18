//
// Created by Master on 14/06/2019.
//
#include "main.h"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth), screenHeight(screenHeight) {
    TCODConsole::initRoot(screenWidth, screenHeight, "libtcod C++ tutorial", false);
    player = new Actor(40, 25, '@', "player", TCODColor::white);        //player character, color and name
    player->destructible = new PlayerDestructible(30,2,"your carcace"); //what players corpse displays for a name
    player->attacker=new Attacker(5);                                   //give player some attack
    player->ai = new PlayerAi();
    player->container = new Container(26);
    actors.push(player);
    map = new Map(80, 45);
    gui = new Gui();
    gui->message(TCODColor::red, "Welcome marine!\nPrepare to perish in the Hulk of the Hiethan.");
}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
    delete gui;
}

void Engine::update() {
    if (gameStatus == STARTUP) map->computeFov();
    gameStatus = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);
    player->update();
    if( gameStatus == NEW_TURN ){
        for (auto *a : actors){
            if( a != player ){
                a->update();
            }
        }
    }
}

void Engine::render() {
    //clear the console
    TCODConsole::root->clear();

    //draw the map
    map->render();

    //draw the actors
    for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++) {
        Actor *actor = *iterator;
        if (map->isInFov(actor->x, actor->y)) {
            actor->render();
        }
    }
    player->render();
    gui -> render();

    TCODConsole::root->printf(1,screenHeight-2, "Hp: %d/%d",(int)player->destructible->hp, (int)player->destructible->maxHp);
}

void Engine::sendtoBack(Actor *actor) {
    actors.remove(actor);
    actors.insertBefore(actor, 0);
}
