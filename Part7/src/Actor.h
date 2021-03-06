//
// Created by Master on 13/06/2019.
//

#ifndef MOGUE_ACTOR_H
#define MOGUE_ACTOR_H

using namespace std;

class Actor {
public:
    int x, y;//position

    int ch; //ascii code

    TCODColor col;

    const char *name;

    bool blocks;    //can other actors pass through?

    Attacker *attacker; //something that deals damage?

    Destructible *destructible; //something that can be damaged?

    Ai *ai; //self updating actor

public:
    Actor(int x, int y, int ch, const TCODColor &col);

    Actor(int x, int y, int ch, const char *name, const TCODColor &col);

    void update();

    //handled by Ai now
//    bool moveOrAttack(int x, int y);

    void render() const;
};


#endif //MOGUE_ACTOR_H
