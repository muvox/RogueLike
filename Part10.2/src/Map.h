//
// Created by Master on 13/06/2019.
//

#ifndef MOGUE_MAP_H
#define MOGUE_MAP_H

struct Tile {
    bool explored; //toggle if player has seen the tile
    Tile() : explored(false) {}
};

class Map : public Persistent {

    static const int ROOM_MAX_SIZE = 12;
    static const int ROOM_MIN_SIZE = 6;

public:
    int width, height;

    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    void render() const;
    bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    void computeFov();
    bool canWalk(int x, int y) const;
    void addMonster(int x, int y);
    void addItem(int x, int y);
    void init(bool withActors);
    void load(TCODZip &zip);
    void save(TCODZip &zip);

protected:
    Tile *tiles;
    TCODMap *map;
    long seed;
    TCODRandom *rng;
    friend class BspListener;
    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);

    void setWall(int x, int y);
};


#endif //MOGUE_MAP_H
