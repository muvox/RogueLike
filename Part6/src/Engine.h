//
// Created by Master on 14/06/2019.
//

#ifndef MOGUE_ENGINE_H
#define MOGUE_ENGINE_H


class Engine {
public:
    int screenWidth;
    int screenHeight;
    TCOD_key_t lastKey;
    int fovRadius;
    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
    enum GameStatus{
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;

public:
    Engine(int screenWidth, int screenHeight);
    ~Engine();
    void update();
    void render();
    void sendtoBack(Actor *actor);

private:
    bool computeFov;
};

extern Engine engine;
#endif //MOGUE_ENGINE_H
