//
// Created by Master on 14/06/2019.
//

#ifndef MOGUE_ENGINE_H
#define MOGUE_ENGINE_H


class Engine {
public:
    TCODList<Actor *> actors;
    Actor *player;
    Map *map;

public:
    Engine();
    ~Engine();
    void update();
    void render();
};

extern Engine engine;
#endif //MOGUE_ENGINE_H
