//
// Created by Master on 13/06/2019.
//

#ifndef MOGUE_ACTOR_H
#define MOGUE_ACTOR_H

using namespace std;

class Actor : public Persistent{
public:
    int x, y;//position
    int ch; //ascii code
    TCODColor col;
    const char *name;
    bool blocks;    //can other actors pass through?
    Attacker *attacker; //something that deals damage?
    Destructible *destructible; //something that can be damaged?
    Ai *ai; //self updating actor
    Pickable *pickable;
    Container *container;

    Actor(int x, int y, int ch, const char *name, const TCODColor &col);
    ~Actor();
    void update();
    void render() const;
    float getDistance(int cx, int cy) const;
    void load(TCODZip &zip);
    void save(TCODZip &zip);
};


#endif //MOGUE_ACTOR_H
