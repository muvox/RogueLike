//
// Created by Master on 14/06/2019.
//

#ifndef MOGUE_ENGINE_H
#define MOGUE_ENGINE_H


class Engine {
public:
    int screenWidth;
    int screenHeight;
    Gui *gui;
    TCOD_key_t lastKey;
    TCOD_mouse_t mouse;
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
    void sendToBack(Actor *actor);
    Actor *getActor(int x, int y) const;
    Actor *getClosestMonster(int x, int y, float range) const;
    bool pickATile(int *x, int *y, float maxRange = 0.0f);
    void init();
    void terminate();
    void load();
    void save();


private:
    bool computeFov;
};

extern Engine engine;
#endif //MOGUE_ENGINE_H
