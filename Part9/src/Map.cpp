//
// Created by Master on 13/06/2019.
//
#include "main.h"


//set some static colors
static const TCODColor darkWall(0, 0, 100);
static const TCODColor darkGround(50, 50, 150);
static const TCODColor lightWall(130, 110, 50);
static const TCODColor lightGround(200, 180, 50);
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];      //create an array of tiles
    map = new TCODMap(width, height);
    TCODBsp bsp(0, 0, width, height);      //new bsp
    bsp.splitRecursive(nullptr, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, nullptr);
}

Map::~Map() {
    delete[] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const {
    return !map->isWalkable(x, y);
}

bool Map::canWalk(int x, int y) const {
    if (isWall(x, y)) {
        return false;
    }

    for (auto *a : engine.actors) {
        if (a->blocks && a->x == x && a->y == y) {
            //blocking actor, cant walk
            return false;
        }
    }

//    for (Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) {
//        Actor *actor = *iterator;
//        if (actor->x == x && actor->y == y) {
//            //there is an actor here, cannot walk here.
//            return false;
//        }
//
//    }

    return true;
}

void Map::addMonster(int x, int y) {
    TCODRandom *rng = TCODRandom::getInstance();
    if (rng->getInt(0, 100) < 80) {
        //create and orc, ~ 80% chance
        Actor *orc = new Actor(x, y, 'o', "orc", TCODColor::desaturatedGreen);
        orc->destructible = new MonsterDestructible(10, 0, "dead orc");
        orc->attacker = new Attacker(3);
        orc->ai = new MonsterAi();
        engine.actors.push(orc);
    } else {
        //create a troll
        Actor *troll = new Actor(x, y, 'T', "troll", TCODColor::darkerGreen);
        troll->destructible = new MonsterDestructible(16, 1, "troll carcass");
        troll->attacker = new Attacker(4);
        troll->ai = new MonsterAi();
        engine.actors.push(troll);
    }
}

void Map::setWall(int x, int y) {
    map->setProperties(x, y, false, false);
}

bool Map::isExplored(int x, int y) const {
    return tiles[x + y * width].explored;
}

bool Map::isInFov(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    if (map->isInFov(x, y)) {
        tiles[x + y * width].explored = true;
        return true;
    }
    return false;
}

void Map::computeFov() {
    map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

void Map::dig(int x1, int y1, int x2, int y2) {

    /*!Visualisation for myself
        x1      x2
    .   .   .   .   .   .   .
 y1 .   #   .   #   .   .   .
    .   .   .   .   .   .   .
    .   .   .   .   .   .   .
 y2 .   #   .   #   .   .   .
    .   .   .   .   .   .   .
    */
    //swap the coordinates we wont try to draw
    if (x2 < x1) {
        int tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    if (y2 < y1) {
        int tmp = y2;
        y2 = y1;
        y1 = tmp;
    }

    for (int tilex = x1; tilex <= x2; tilex++) {
        for (int tiley = y1; tiley <= y2; tiley++) {
            map->setProperties(tilex, tiley, true, true);
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
    dig(x1, y1, x2, y2);
    if (first) {
        //place player in the first room
        engine.player->x = (x1 + x2) / 2;
        engine.player->y = (y1 + y2) / 2;
    } else {
        TCODRandom *rng = TCODRandom::getInstance();
        int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (nbMonsters > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (canWalk(x, y)) {
                addMonster(x, y);
            }
            nbMonsters--;
        }


        int nbItems = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (nbItems > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (canWalk(x, y)) {
                addItem(x, y);
            }
            nbItems--;
        }
    }
    //add items

}

void Map::addItem(int x, int y) {
    TCODRandom *rng = TCODRandom::getInstance();
    int dice = rng->getInt(0, 100);
    if (dice < 70) {
        //create health potion
        Actor *healthPotion = new Actor(x, y, '!', "health potion", TCODColor::violet);
        healthPotion->blocks = false;
        healthPotion->pickable = new Healer(4);
        engine.actors.push(healthPotion);
    } else if (dice < 70 + 10) {
        //create lightning bolt
        Actor *scrollOfLightningBolt = new Actor(x, y, '#', "Scroll of Lightning Bolt", TCODColor::lightYellow);
        scrollOfLightningBolt->blocks = false;
        scrollOfLightningBolt->pickable = new LightningBolt(5, 20);
        engine.actors.push(scrollOfLightningBolt);
    } else if (dice > 70 + 10 + 10) {
        //create fireball scrolls
        Actor *scrollOfFireball = new Actor(x, y, '#', "Scroll of Fireball", TCODColor::lightYellow);
        scrollOfFireball->blocks = false;
        scrollOfFireball->pickable = new Fireball(3, 12);
        engine.actors.push(scrollOfFireball);
    } else {
        Actor *scrollOfConfusion = new Actor(x, y, '#', "Scroll of Confusion", TCODColor::lightYellow);
        scrollOfConfusion->blocks = false;
        scrollOfConfusion->pickable = new Confuser(10, 8);
        engine.actors.push(scrollOfConfusion);
    }
}

void Map::render() const {


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (isInFov(x, y)) {
                TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? lightWall : lightGround);
            } else if (isExplored(x, y)) {
                TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? darkWall : darkGround);
            }
//            TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? darkWall : darkGround);
        }
    }
}