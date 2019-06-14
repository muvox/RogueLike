//
// Created by Master on 13/06/2019.
//

#ifndef MOGUE_MAP_H
#define MOGUE_MAP_H

struct Tile {
    bool canWalk;
    Tile() : canWalk(true) {}
};

class Map {
public:
    int width, height;

    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    void render() const;
protected:
    Tile *tiles;

    void setWall(int x, int y);
};


#endif //MOGUE_MAP_H
